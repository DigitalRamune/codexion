*This project has been created as part of the 42 curriculum by inaciri.*

# Codexion

## Description
**Codexion** is a C project that explores advanced multithreading, concurrency, and shared resource management. Inspired by the classic Dining Philosophers problem, this project simulates a group of developers (coders) sitting around a table who must share a limited number of USB drives (dongles) to compile, debug, and refactor their code. 

The goal of the project is to build a robust, thread-safe simulation that efficiently allocates limited resources without triggering deadlocks or starvation. It features dynamic resource scheduling using a Min-Heap priority queue, allowing the system to operate under two distinct strategies: **FIFO** (First In, First Out) and **EDF** (Earliest Deadline First).

## Instructions

### Compilation
The project uses a standard `Makefile`. Run the following commands in the root of the repository:

```bash
make        # Compiles the 'codexion' executable
make clean  # Removes object files (.o)
make fclean # Removes object files and the executable
make re     # Fully recompiles the project

```

### Execution

The simulation requires 8 mandatory arguments:

```bash
./codexion [coders] [time_to_burn] [compile] [debug] [refactor] [quota] [cooldown] [scheduler]

```

**Arguments:**

1. `number of coders`: Number of developers (and dongles).
2. `time to burn`: Time (in ms) a developer can wait without compiling before making a "burnout" (dying).
3. `time to compile`: Time (in ms) spent compiling (requires acquiring 2 adjacent dongles).
4. `time to debug`: Time (in ms) spent debugging after releasing dongles.
5. `time to refactor`: Time (in ms) spent refactoring the code.
6. `number of required compilations`: Quota of compilations each coder must reach for the simulation to end successfully.
7. `dongle cooldown`: Time (in ms) a dongle remains unusable after being released.
8. `scheduler`: The queue scheduling strategy (`fifo` or `edf`).

**Example:**

```bash
./codexion 4 430 200 0 0 5 10 fifo

```

## Resources

* **Documentation & Articles:**
* [POSIX Threads Programming (LLNL)](https://www.google.com/search?q=https://computing.llnl.gov/tutorials/pthreads/)
* [Dining Philosophers Problem & Coffman's Conditions](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Earliest Deadline First Scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)


* **AI Usage:**
* AI (Gemini) was used during the development of this project as an interactive tutor and debugging assistant. Specifically, it was used to:
* Refine the mathematical logic behind the Min-Heap priority queue for the EDF and FIFO schedulers.
* Analyze and resolve complex deadlock scenarios and starvation edge cases (e.g., odd-numbered coder configurations).
* Interpret `ThreadSanitizer` reports to locate and patch subtle Data Races.
* Refactor specific functions to comply with the strict 42 *Norminette* constraints (line limits and variable counts) without losing readability.





## Blocking cases handled

To ensure the simulation runs flawlessly, several critical concurrency issues were addressed:

* **Deadlock prevention and Coffman’s conditions:** To break the "circular wait" condition (one of Coffman's conditions for deadlock), a strict resource acquisition hierarchy is enforced. Even-ID coders must request their right dongle first, while odd-ID coders must request their left dongle first. This guarantees that at least one coder will always be able to pick up both dongles, preventing the entire table from freezing.
* **Starvation prevention:** A Min-Heap priority queue is implemented within each dongle. Coders do not randomly grab dongles; they insert their requests into the heap. Depending on the chosen scheduler, the dongle is granted strictly to the coder who requested it first (FIFO) or the coder closest to burnout (EDF), ensuring no one is permanently bypassed.
* **Cooldown handling:** Dongles must cool down after use. This is handled by a precise timestamp check upon acquisition. If a coder is granted a dongle but the cooldown period hasn't elapsed, the thread will accurately calculate the remaining time and `usleep` for that specific duration before attempting to use it.
* **Precise burnout detection:** A dedicated Monitor thread continuously loops over all coders, calculating the difference between the current time and their `last_compile` timestamp. If this difference exceeds `time_to_burn`, the monitor instantly sets a global stop flag and halts the simulation to print the death message accurately.
* **Log serialization:** Multiple threads attempting to print to the standard output simultaneously can result in interlaced or corrupted text. A dedicated `m_print` mutex locks the printing function, ensuring that timestamps and state changes are printed sequentially and coherently.

## Thread synchronization mechanisms

The architecture relies entirely on POSIX threading primitives to coordinate access to shared resources safely:

* **`pthread_mutex_t`:** Mutexes are deployed to protect every shared variable from Data Races.
* `m_dongle` protects the dongle's state (whether it is used, its cooldown timestamp) and its internal Min-Heap array.
* `m_flag` securely wraps the global `stop_flag`. No thread reads or writes this flag without locking the mutex (e.g., via the `check_stop_flag` getter), preventing the monitor and coders from conflicting during a shutdown.
* `m_comp` protects individual coder states (`compilations` count and `last_compile` timestamp) so the Monitor can read them safely while the coder thread is updating them.


* **`pthread_cond_t`:** Condition variables (`cond_dongle`) are used for passive synchronization, saving CPU cycles compared to aggressive spinlocks. When a coder needs a dongle but is not yet the highest priority in the heap (or the dongle is in use), they call `pthread_cond_wait`.
* **Thread-safe communication:** When a coder finishes compiling and releases a dongle, they unlock `m_dongle` and immediately trigger `pthread_cond_broadcast`. This sends a signal to all asleep threads waiting for that specific dongle. The waiting threads wake up, re-lock the mutex, and check their condition (via `cond_check`) to see if they are now the highest priority in the Min-Heap. Furthermore, if the Monitor detects a burnout or completion, it sets the `stop_flag` and broadcasts to all dongle condition variables to immediately wake up any sleeping coders so they can gracefully exit their routines.
