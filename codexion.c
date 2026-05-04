/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/04 16:15:51 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

struct Code_data {
	int				id;
	int				dongles;
	int				comp_time;
	int				debug_time;
	int				ref_time;
	int				burn_time;
	struct timeval	tv_last;
	struct timeval	tv_start;
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

	self_data = (struct Code_data *)argument;
	i = 0;
	while (i < self_data->dongles)
	{
		gettimeofday(&tv_now, NULL);
		printf("%ld %d has taken a dongle\n", (tv_now.tv_usec - self_data->tv_start.tv_usec), self_data->id);
		i++;
	}
	gettimeofday(&tv_now, NULL);
	self_data->tv_last = tv_now;
	printf("%ld %d is compiling\n", (self_data->tv_last.tv_usec - (self_data->tv_start.tv_usec)), self_data->id);
	gettimeofday(&tv_now, NULL);
	printf("%ld %d is debugging\n", (tv_now.tv_usec - (self_data->tv_start.tv_usec)), self_data->id);
	gettimeofday(&tv_now, NULL);
	printf("%ld %d is refactoring\n", (tv_now.tv_usec - (self_data->tv_start.tv_usec)), self_data->id);
	return NULL;
}

int	main(int argc, char **argv)
{
	int					i;
	struct Code_data	*all_code;
	struct timeval		tv;
	pthread_t			*code_threads;
	int					args[8];

	if (!valid_data(args, argc, argv))
		return 0;
	// --------------------------

	all_code = malloc(args[0] * sizeof(struct Code_data));
	code_threads = malloc(args[0] * sizeof(pthread_t));
	if (!all_code)
	{
		free(all_code);
		return 0;
	}
	i = 0;
	gettimeofday(&tv, NULL);
	while (i < args[0])
	{
		all_code[i].id = i + 1;
		all_code[i].tv_start = tv;
		if (args[0] == 1)
			all_code[i].dongles = 1;
		else
			all_code[i].dongles = 2;
		all_code[i].burn_time = args[1];
		all_code[i].comp_time = args[2];
		all_code[i].burn_time = args[3];
		all_code[i].ref_time = args[4];
		pthread_create(&code_threads[i], NULL, coders_step, &all_code[i]);
		pthread_join(code_threads[i], NULL);
		i++;
	}
}