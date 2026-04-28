/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/28 18:32:44 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct Code_data {
	int				id;
	struct timeval	tv_last;
	struct timeval	tv_start;
};

void* coders_step(void* argument)
{
	struct timeval tv_now;
	struct Code_data *self_data;

	self_data = (struct Code_data *)argument;
	gettimeofday(&tv_now, NULL);
	self_data->tv_last = tv_now;
	printf("Temps passe depuis le debut: %ldms\n", (self_data->tv_last.tv_usec - (self_data->tv_start.tv_usec)));
	printf("Le thread %d a fini l'execution a %ldms\n", self_data->id, self_data->tv_last.tv_usec);
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
	
	gettimeofday(&tv, NULL);
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
	while (i < nbr_coders)
	{
		all_code[i].id = i;
		all_code[i].tv_start = tv;
		pthread_create(&code_threads[i], NULL, coders_step, &all_code[i]);
		pthread_join(code_threads[i], NULL);
		i++;
	}
}