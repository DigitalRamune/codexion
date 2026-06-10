/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri < inaciri@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:30:49 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/10 20:43:40 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <string.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct P_arg
{
	int		nbr_of_coders;
	int		time_to_burn;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		required_compiles;
	int		dongle_cooldown;
	char	*scheduler;
}			t_arg;

typedef struct S_sim
{
	int				stop_flag;
	pthread_mutex_t	m_flag;
	pthread_mutex_t	m_print;
	struct timeval	start;
	struct D_dongle	*dongle_tab;
	struct C_coder	*coders_tab;
	t_arg			*param;
}					t_sim;

typedef struct R_request
{
	int				id;
	struct timeval	request_time;
	struct timeval	deadline;
}					t_request;

typedef struct D_dongle
{
	pthread_mutex_t		m_dongle;
	pthread_cond_t		cond_dongle;
	int					is_used;
	t_request			*heap;
	int					in_heap;
	int					max_coders;
	struct timeval		cooldown;
	struct timeval		released_at;
}						t_dongle;

typedef struct C_coder
{
	int				id;
	int				compilations;
	t_dongle		*left;
	t_dongle		*right;
	struct timeval	last_compile;
	t_sim			*simulation;
	pthread_mutex_t	m_comp;
}					t_coder;

int				ft_parsing(char *data, char **argv);
int				error_return(int z, char *arg_name);
int				valid_data(int *data, int argc, char **argv);
int				fill_struct(int argc, char **argv, t_arg *param);

int				ft_edf_compare(t_request req1, t_request req2);
int				ft_compare(t_sim *simulation, t_request req1, t_request req2);
void			heap_bubble_up(t_sim *simulation, t_dongle *dongle, t_request new_req);
t_request		heap_bubble_down(t_sim *simulation, t_dongle *dongle);
void			heap_insert(t_sim *sim, t_dongle *dong, t_coder *cod);
t_request		heap_extract(t_sim *sim, t_dongle *dongle);
t_request		check_min(t_dongle *dongle);

int				init_dongle_tab(t_sim *sim);
int				init_coders_tab(t_sim *sim);
int				init_simulation(t_sim *sim, t_arg *arg);

void			cooldown_convert(t_dongle *dongle, t_sim *sim);
int				init_dongle_heap(t_dongle *dongle);
int				init_dongle(t_dongle *dongle, t_sim *sim);
int				add_all_dongle(t_sim *sim);

void			init_coder(t_coder *cod, t_sim *sim, int id);
void			add_all_coders(t_sim *sim);

int				timeval_compare(struct timeval a, struct timeval b);
struct timeval	timeval_add(struct timeval a, struct timeval b);

int				one_coder(t_sim *sim);

int				is_min(t_dongle *dong, t_coder *cod);
int				cond_check(t_dongle *dong, t_coder *cod);
void			dongle_acquisition(t_dongle *dong, t_coder *cod, t_sim *sim);
void			dongle_liberation(t_sim *sim, t_dongle *dong);

void			print_dong(t_sim *sim, t_coder *cod);
void			print_comp(t_sim *sim, t_coder *cod);
void			print_debug(t_sim *sim, t_coder *cod);
void			print_refactor(t_sim *sim, t_coder *cod);
void			print_burn(t_sim *sim, t_coder *cod);

void			*cod_main(void *arg);
void			*ft_monitor(void *arg);
void			free_tab(t_sim *sim);
#endif