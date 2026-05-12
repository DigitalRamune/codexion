/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/11 16:43:06 by inaciri          ###   ########.fr       */
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

struct Code_data {
	int				id;
	int				comp_nbr;
	useconds_t		comp_time;
	useconds_t		debug_time;
	useconds_t		ref_time;
	useconds_t		burn_time;
	struct timeval	tv_last;
	struct timeval	tv_start;
	pthread_mutex_t	*l_dongle;
	pthread_mutex_t *r_dongle;
	pthread_mutex_t *state_mutex;
	pthread_mutex_t *start_mutex;
};

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
		if (data[i] == 0)
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
		if (self_data->id % 2 == 0)
		{
			pthread_mutex_lock(self_data->l_dongle);
			pthread_mutex_lock(self_data->start_mutex);
			printf("%ld %d has taken a dongle\n", clc_time(self_data->tv_start), self_data->id);
			pthread_mutex_unlock(self_data->start_mutex);
			pthread_mutex_lock(self_data->r_dongle);
			pthread_mutex_lock(self_data->start_mutex);
			printf("%ld %d has taken a dongle\n", clc_time(self_data->tv_start), self_data->id);
			pthread_mutex_unlock(self_data->start_mutex);
		}
		else
		{
			pthread_mutex_lock(self_data->r_dongle);
			pthread_mutex_lock(self_data->start_mutex);
			printf("%ld %d has taken a dongle\n", clc_time(self_data->tv_start), self_data->id);
			pthread_mutex_unlock(self_data->start_mutex);
			pthread_mutex_lock(self_data->l_dongle);
			pthread_mutex_lock(self_data->start_mutex);
			printf("%ld %d has taken a dongle\n", clc_time(self_data->tv_start), self_data->id);
			pthread_mutex_unlock(self_data->start_mutex);
		}
		gettimeofday(&tv_now, NULL);
		
		self_data->tv_last = tv_now;
		time_ms = (self_data->tv_last.tv_sec - self_data->tv_start.tv_sec) * 1000 + (self_data->tv_last.tv_usec - self_data->tv_start.tv_usec) / 1000;
		pthread_mutex_lock(self_data->start_mutex);
		printf("%ld %d is compiling\n", time_ms, self_data->id);
		pthread_mutex_unlock(self_data->start_mutex);
		usleep(self_data->comp_time);
		pthread_mutex_unlock(self_data->l_dongle);
		pthread_mutex_unlock(self_data->r_dongle);
		
		pthread_mutex_lock(self_data->state_mutex);
		self_data->comp_nbr++;
		pthread_mutex_unlock(self_data->state_mutex);
		
		pthread_mutex_lock(self_data->start_mutex);
		printf("%ld %d is debugging\n", clc_time(self_data->tv_start), self_data->id);
		pthread_mutex_unlock(self_data->start_mutex);
		usleep(self_data->debug_time);
		
		pthread_mutex_lock(self_data->start_mutex);
		printf("%ld %d is refactoring\n", clc_time(self_data->tv_start), self_data->id);
		pthread_mutex_unlock(self_data->start_mutex);
		usleep(self_data->ref_time);
		return NULL;
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	int					i;
	int					finished_coders;
	int					args[8];
	long				time_ms;
	pthread_t			*code_threads;
	pthread_mutex_t		*dongles_tab;
	pthread_mutex_t		*comp_nbr_tab;
	pthread_mutex_t		*start_tab;
	struct Code_data	*all_code;
	struct timeval		tv;

	if (!valid_data(args, argc, argv))
		return 0;
	// --------------------------

	all_code = malloc(args[0] * sizeof(struct Code_data));
	code_threads = malloc(args[0] * sizeof(pthread_t));
	dongles_tab = malloc(args[0] * sizeof(pthread_mutex_t));
	comp_nbr_tab = malloc(args[0] * sizeof(pthread_mutex_t));
	start_tab = malloc(args[0] * sizeof(pthread_mutex_t));
	
	if (!all_code || !code_threads || !dongles_tab || !comp_nbr_tab)
	{
		free(all_code);
		free(code_threads);
		free(dongles_tab);
		free(comp_nbr_tab);
		return 0;
	}
	i = 0;
	
	while (i < args[0])
	{
		pthread_mutex_init(&dongles_tab[i], NULL);
		pthread_mutex_init(&comp_nbr_tab[i], NULL);
		pthread_mutex_init(&start_tab[i], NULL);
		i++;
	}
	gettimeofday(&tv, NULL);
	
	i = 0;
	while (i < args[0])
	{
		all_code[i].id = i + 1;
		all_code[i].tv_start = tv;
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
		all_code[i].start_mutex = &start_tab[i];
		pthread_create(&code_threads[i], NULL, coders_step, &all_code[i]);
		i++;
	}
	
	while(1)
	{
		i = 0;
		finished_coders = 0;
		
		while (i < args[0])
		{
			pthread_mutex_lock(all_code[i].state_mutex);
			
			gettimeofday(&tv, NULL);
			pthread_mutex_lock(all_code[i].start_mutex);
			time_ms = clc_time(all_code[i].tv_last);
			pthread_mutex_unlock(all_code[i].start_mutex);
			if (time_ms > args[1])
			{
				printf("%d\n", args[1]);
				gettimeofday(&tv, NULL);
				pthread_mutex_lock(all_code[i].start_mutex);
				printf("%ld %d burned out\n", clc_time(all_code[i].tv_start), all_code[i].id);
				pthread_mutex_unlock(all_code[i].start_mutex);
				pthread_mutex_unlock(all_code[i].state_mutex);
				break;
			}
			pthread_mutex_unlock(all_code[i].state_mutex);
			i++;
		}
		
	}

	i = 0;
	while (i < args[0])
	{
		pthread_join(code_threads[i], NULL);
		i++;
	}
}