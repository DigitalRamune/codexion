/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/29 14:57:05 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct Code_data {
	int				id;
	int				dongles;
	struct timeval	tv_last;
	struct timeval	tv_start;
};

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
	// Validation nbr of coders
	int					nbr_coders;
	int					i;
	struct Code_data	*all_code;
	struct timeval		tv;
	pthread_t			*code_threads;
	
	nbr_coders = atoi(argv[1]);
	(void) argc;
	if (nbr_coders <= 0)
	{
		printf("Please enter a valid number of coders\n");
		return 0;
	}
	// --------------------------

	all_code = malloc(nbr_coders * sizeof(struct Code_data));
	code_threads = malloc(nbr_coders * sizeof(pthread_t));
	if (!all_code)
	{
		free(all_code);
		return 0;
	}
	i = 0;
	gettimeofday(&tv, NULL);
	while (i < nbr_coders)
	{
		all_code[i].id = i + 1;
		all_code[i].tv_start = tv;
		if (nbr_coders == 1)
			all_code[i].dongles = 1;
		else
			all_code[i].dongles = 2;
		pthread_create(&code_threads[i], NULL, coders_step, &all_code[i]);
		pthread_join(code_threads[i], NULL);
		i++;
	}
}