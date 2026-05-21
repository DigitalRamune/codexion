/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/20 17:15:24 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _DEFAULT_SOURCE

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

long	clc_time(struct timeval time_s)
{
	long			time_ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time_ms = (tv.tv_sec - time_s.tv_sec) * 1000 + (tv.tv_usec - time_s.tv_usec) / 1000;
	return time_ms;
}

struct	Dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_used;
	int				*id_list;
	int				head;
	int				list_size;
	int				total_coders;
	int				tail;	
};

struct Code_data {
	int				id;
	int				comp_nbr;
	int				*stop_flag;
	useconds_t		comp_time;
	useconds_t		debug_time;
	useconds_t		ref_time;
	useconds_t		burn_time;
	struct timeval	tv_last;
	struct timeval	tv_start;
	pthread_mutex_t *state_mutex;
	pthread_mutex_t	*p_mutex;
	pthread_mutex_t *s_mutex;
	struct Dongle	*l_dongle;
	struct Dongle	*r_dongle;
};

void	take_dongle(struct Dongle *dongle, int id)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->id_list[dongle->tail] = id;
	dongle->list_size += 1;
	dongle->tail = (dongle->tail + 1) % (dongle->total_coders);
	while (dongle->is_used == 1 || dongle->id_list[dongle->head] != id)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	dongle->is_used = 1;
	dongle->list_size -= 1;
	dongle->head = (dongle->head + 1) % (dongle->total_coders);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongle(struct Dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_used = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	argument_names(char **arg_tab)
{
	arg_tab[0] = "number of coders";
	arg_tab[1] = "time to burn";
	arg_tab[2] = "time to compile";
	arg_tab[3] = "time to debug";
	arg_tab[4] = "time to refactor";
	arg_tab[5] = "number of required compilations";
	arg_tab[6] = "dongle cooldown";
	arg_tab[7] = "scheduler";
}

void	safe_print(struct Code_data *data, char *status_s)
{
	pthread_mutex_lock(data->p_mutex);
	pthread_mutex_lock(data->s_mutex);
	if (*(data->stop_flag) == 0)
	{
		printf("%ld %d %s\n", clc_time(data->tv_start), data->id, status_s);
	}
	pthread_mutex_unlock(data->p_mutex);
	pthread_mutex_unlock(data->s_mutex);
}

int	valid_data(int *data, int argc, char **argv)
{
	int i;
	char *arg_name[8];
	
	argument_names(arg_name);
	i = 0;
	if (argc < 9 || argc > 9)
	{
		printf("Invalid number of arguments given\n");
		return 0;
	}
	while (i < 7)
	{
		data[i] = atoi(argv[i + 1]);
		if (data[i] < 0)
		{
			printf("Invalid %s\n", arg_name[i]);
			return 0;
		}
		i++;
	}
	if (!strcmp(argv[8], "FIFO") || !strcmp(argv[8], "fifo"))
		data[i] = 0;
	else if (!strcmp(argv[8], "EDF") || !strcmp(argv[8], "edf"))
		data[i] = 1;
	else
	{
		printf("Invalid %s\n", arg_name[i]);
		return 0;		
	}
	return 1;
}

int	cmp_nbr(struct Code_data *all_code, int cmp_nbr, int code_nbr)
{
	int	i;
	int	cmp;
	
	i = 0;
	cmp = 0;
	while (i < code_nbr)
	{
		pthread_mutex_lock(all_code[i].state_mutex);
		if (all_code[i].comp_nbr == cmp_nbr)
			cmp += 1;
		pthread_mutex_unlock(all_code[i].state_mutex);
		i++;
	}
	if (cmp == cmp_nbr)
		return (0);
	return (1);
}


void* coders_step(void* argument)
{
	struct timeval		tv_now;
	struct Code_data	*self_data;
	int					i;
	long				time_ms;
	
	self_data = (struct Code_data *)argument;
	i = 0;
	while(1)
	{
		pthread_mutex_lock(self_data->s_mutex);
		if (*(self_data->stop_flag) == 1)
		{
			pthread_mutex_unlock(self_data->s_mutex);
			break;
		}
		pthread_mutex_unlock(self_data->s_mutex);
		
		if (self_data->id % 2 == 0)
		{
			take_dongle(self_data->l_dongle, self_data->id);
			safe_print(self_data, "has taken a dongle");
			take_dongle(self_data->r_dongle, self_data->id);
			safe_print(self_data, "has taken a dongle");
		}
		else
		{
			take_dongle(self_data->r_dongle, self_data->id);
			safe_print(self_data, "has taken a dongle");
			take_dongle(self_data->l_dongle, self_data->id);
			safe_print(self_data, "has taken a dongle");
		}
		gettimeofday(&tv_now, NULL);
		pthread_mutex_lock(self_data->state_mutex);
		self_data->tv_last = tv_now;
		time_ms = (self_data->tv_last.tv_sec - self_data->tv_start.tv_sec) * 1000 + (self_data->tv_last.tv_usec - self_data->tv_start.tv_usec) / 1000;
		pthread_mutex_unlock(self_data->state_mutex);
		safe_print(self_data, "is compiling");
		usleep(self_data->comp_time);
		release_dongle(self_data->l_dongle);
		release_dongle(self_data->r_dongle);
		
		pthread_mutex_lock(self_data->state_mutex);
		self_data->comp_nbr++;
		pthread_mutex_unlock(self_data->state_mutex);
		
		safe_print(self_data, "is debugging");
		usleep(self_data->debug_time);
		
		safe_print(self_data, "is refactoring");
		usleep(self_data->ref_time);
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	int					i;
	int					stop_flag;
	int					finished_coders;
	int					simulate_running;
	int					args[8];
	long				time_ms;
	pthread_t			*code_threads;
	pthread_mutex_t		print_mutex;
	struct Dongle		*dongles_tab;
	pthread_mutex_t		*comp_nbr_tab;
	pthread_mutex_t		stop_mutex;
	struct Code_data	*all_code;
	struct timeval		tv;

	if (!valid_data(args, argc, argv))
		return 0;
	// --------------------------

	all_code = malloc(args[0] * sizeof(struct Code_data));
	code_threads = malloc(args[0] * sizeof(pthread_t));
	dongles_tab = malloc(args[0] * sizeof(struct Dongle));
	comp_nbr_tab = malloc(args[0] * sizeof(pthread_mutex_t));
	
	if (!all_code || !code_threads || !dongles_tab || !comp_nbr_tab)
	{
		free(all_code);
		free(code_threads);
		free(dongles_tab);
		free(comp_nbr_tab);
		return 0;
	}
	i = 0;
	stop_flag = 0;
	simulate_running = 1;
	while (i < args[0])
	{
		pthread_mutex_init(&dongles_tab[i].mutex, NULL);
		pthread_mutex_init(&comp_nbr_tab[i], NULL);
		pthread_cond_init(&dongles_tab[i].cond, NULL);
		dongles_tab[i].total_coders = args[0];
		dongles_tab[i].is_used = 0;
		dongles_tab[i].head = 0;
		dongles_tab[i].tail = 0;
		dongles_tab[i].list_size = 0;
		dongles_tab[i].id_list = malloc(args[0] * sizeof(int));
		if (!dongles_tab[i].id_list)
			return 0;
		i++;
	}
	pthread_mutex_init(&print_mutex, NULL);
	pthread_mutex_init(&stop_mutex, NULL);
	gettimeofday(&tv, NULL);
	
	i = 0;
	while (i < args[0])
	{
		all_code[i].id = i + 1;
		all_code[i].tv_start = tv;
		all_code[i].tv_last = tv;
		all_code[i].burn_time = args[1];
		all_code[i].comp_time = args[2] * 1000;
		all_code[i].debug_time = args[3] * 1000;
		all_code[i].ref_time = args[4] * 1000;
		if (i == (args[0] - 1))
			all_code[i].r_dongle = &dongles_tab[0];
		else
			all_code[i].r_dongle = &dongles_tab[i + 1];
		all_code[i].l_dongle = &dongles_tab[i];
		all_code[i].comp_nbr = 0;
		all_code[i].state_mutex = &comp_nbr_tab[i];
		all_code[i].p_mutex = &print_mutex;
		all_code[i].s_mutex = &stop_mutex;
		all_code[i].stop_flag = &stop_flag;
		pthread_create(&code_threads[i], NULL, coders_step, &all_code[i]);
		i++;
	}
	
	while(simulate_running && cmp_nbr(all_code, args[5], args[0]))
	{
		i = 0;
		finished_coders = 0;
		
		while (i < args[0])
		{
			pthread_mutex_lock(all_code[i].state_mutex);
			
			gettimeofday(&tv, NULL);
			time_ms = clc_time(all_code[i].tv_last);
			if (time_ms > args[1])
			{
				printf("%d\n", args[1]);
				gettimeofday(&tv, NULL);
				pthread_mutex_lock(all_code[i].p_mutex);
				printf("%ld %d burned out\n", clc_time(all_code[i].tv_start), all_code[i].id);
				pthread_mutex_unlock(all_code[i].state_mutex);
				pthread_mutex_lock(all_code[i].s_mutex);
				stop_flag = 1;
				pthread_mutex_unlock(all_code[i].s_mutex);
				pthread_mutex_unlock(all_code[i].p_mutex);
				simulate_running = 0;
				break;
			}
			pthread_mutex_unlock(all_code[i].state_mutex);
			i++;
		}
	}
	pthread_mutex_lock(&stop_mutex);
	stop_flag = 1;
	pthread_mutex_unlock(&stop_mutex);

	i = 0;
	while (i < args[0])
	{
		pthread_join(code_threads[i], NULL);
		i++;
	}
}