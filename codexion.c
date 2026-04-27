/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:24:24 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/27 15:16:50 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

void* coders_step(void* argument)
{
	struct timeval tv_now;
	struct timeval *tv_start;

	tv_start = (struct timeval *)argument;
	gettimeofday(&tv_now, NULL);
	printf("Temps passe depuis le debut: %ldms\n", (tv_now.tv_usec - (tv_start->tv_usec)));
	return NULL;
}

int	main(int argc, char **argv)
{
	// Validation nbr of coders
	int	coders;
	int	i;

	struct timeval tv_start;
	gettimeofday(&tv_start, NULL);
	
	coders = atoi(argv[1]);
	(void) argc;
	if (coders <= 0)
	{
		printf("Please enter a valid number of coders\n");
		return 0;
	}
	// --------------------------

	pthread_t code[coders];
	i = 0;
	while (i < coders)
	{
		pthread_create(&code[i], NULL, coders_step, &tv_start);
		pthread_join(code[i], NULL);
		i++;
	}
}