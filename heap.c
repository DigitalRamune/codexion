/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 10:49:27 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/03 15:49:43 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap_utils.c"
#include "parse.c"
#include "codexion.h"

void	data_coders(t_coder *cod, int id, t_dongle *l, t_dongle *r, t_sim *sim)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	cod->id = id;
	cod->compilations = 0;
	cod->left = l;
	cod->right = r;
	cod->last_compile.tv_sec = tv.tv_sec;
	cod->last_compile.tv_usec = tv.tv_usec;
	cod->simulation = sim;
}

void	init_sim(t_sim *sim)
{
	sim->param->time_to_burn = 20;
}

void	init_dongle(t_dongle *dongle)
{
	pthread_mutex_init(&dongle->m_dongle, NULL);
	dongle->is_used = 0;
	dongle->max_coders = 4;
	dongle->heap = malloc(5 * sizeof(t_coder));
}

void	print_heap(t_dongle *dongle)
{
	int i = 0;
	while (i < 4)
	{
		printf("Place %d is id: %d\n", i, dongle->heap[i].id);
		i++;
	}
}
// VERIF MAIN
int main()
{
	t_sim simulation;
	t_arg arguments;
	t_dongle dg1;
	t_dongle dg2;
	t_coder	cd1;
	t_coder	cd2;
	t_coder	cd3;
	t_coder cd4;
	t_request extract;
	struct timeval tv;

	arguments.dongle_cooldown = 10;
	arguments.nbr_of_coders = 4;
	arguments.required_compiles = 2;
	arguments.scheduler = "edf";
	arguments.time_to_burn = 20;
	arguments.time_to_compile = 2;
	arguments.time_to_debug = 2;
	arguments.time_to_refactor = 2;

	simulation.param = &arguments;
	
	init_sim(&simulation);
	init_dongle(&dg1);
	data_coders(&cd1, 0, &dg1, &dg2, &simulation);
	data_coders(&cd2, 1, &dg1, &dg2, &simulation);
	data_coders(&cd3, 2, &dg1, &dg2, &simulation);
	data_coders(&cd4, 3, &dg1, &dg2, &simulation);
	gettimeofday(&tv, NULL);
	cd4.last_compile.tv_sec = tv.tv_sec;
	cd4.last_compile.tv_usec = tv.tv_usec;
	usleep(1000);
	gettimeofday(&tv, NULL);
	cd3.last_compile.tv_sec = tv.tv_sec;
	cd3.last_compile.tv_usec = tv.tv_usec;
	usleep(1000);
	gettimeofday(&tv, NULL);
	cd2.last_compile.tv_sec = tv.tv_sec;
	cd2.last_compile.tv_usec = tv.tv_usec;
	usleep(1000);
	gettimeofday(&tv, NULL);
	cd1.last_compile.tv_sec = tv.tv_sec;
	cd1.last_compile.tv_usec = tv.tv_usec;
	heap_insert(&simulation, &dg1, &cd1);
	usleep(1000);
	heap_insert(&simulation, &dg1, &cd2);
	usleep(1000);
	heap_insert(&simulation, &dg1, &cd3);
	usleep(1000);
	heap_insert(&simulation, &dg1, &cd4);
	print_heap(&dg1);

	extract = heap_extract(&simulation, &dg1);
	int i = 1;
	printf("Extract %d, is id: %d\n", i, extract.id);
	extract = heap_extract(&simulation, &dg1);
	i++;
	printf("Extract %d, is id: %d\n", i, extract.id);
	extract = heap_extract(&simulation, &dg1);
	i++;
	printf("Extract %d, is id: %d\n", i, extract.id);
	extract = heap_extract(&simulation, &dg1);
	i++;
	printf("Extract %d, is id: %d\n", i, extract.id);
}