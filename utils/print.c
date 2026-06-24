/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 13:21:39 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/24 13:13:06 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

void	print_dong(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	long			diff_ms;

	gettimeofday(&tv, NULL);
	diff_ms = ((tv.tv_sec - sim->start.tv_sec) * 1000L)
		+ ((tv.tv_usec - sim->start.tv_usec) / 1000L);
	pthread_mutex_lock(&sim->m_print);
	printf("%ld %d has taken a dongle\n", diff_ms, cod->id);
	pthread_mutex_unlock(&sim->m_print);
}

void	print_comp(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	long			diff_ms;
	long			sleep_time;

	sleep_time = sim->param->time_to_compile * 1000L;
	if (!check_stop_flag(sim))
	{
		gettimeofday(&tv, NULL);
		diff_ms = ((tv.tv_sec - sim->start.tv_sec) * 1000L)
			+ ((tv.tv_usec - sim->start.tv_usec) / 1000L);
		pthread_mutex_lock(&cod->m_comp);
		cod->last_compile.tv_sec = tv.tv_sec;
		cod->last_compile.tv_usec = tv.tv_usec;
		pthread_mutex_unlock(&cod->m_comp);
		pthread_mutex_lock(&sim->m_print);
		printf("%ld %d is compiling\n", diff_ms, cod->id);
		pthread_mutex_unlock(&sim->m_print);
		precise_sleep(sleep_time);
		pthread_mutex_lock(&cod->m_comp);
		cod->compilations += 1;
		pthread_mutex_unlock(&cod->m_comp);
	}
}

void	print_debug(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	long			diff_ms;
	long			sleep_time;

	if (!check_stop_flag(sim))
	{
		sleep_time = sim->param->time_to_debug * 1000L;
		gettimeofday(&tv, NULL);
		diff_ms = ((tv.tv_sec - sim->start.tv_sec) * 1000L)
			+ ((tv.tv_usec - sim->start.tv_usec) / 1000L);
		pthread_mutex_lock(&sim->m_print);
		printf("%ld %d is debugging\n", diff_ms, cod->id);
		pthread_mutex_unlock(&sim->m_print);
		precise_sleep(sleep_time);
	}
}

void	print_refactor(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	long			diff_ms;
	long			sleep_time;

	if (!check_stop_flag(sim))
	{
		sleep_time = sim->param->time_to_refactor * 1000L;
		gettimeofday(&tv, NULL);
		diff_ms = ((tv.tv_sec - sim->start.tv_sec) * 1000L)
			+ ((tv.tv_usec - sim->start.tv_usec) / 1000L);
		pthread_mutex_lock(&sim->m_print);
		printf("%ld %d is refactoring\n", diff_ms, cod->id);
		pthread_mutex_unlock(&sim->m_print);
		precise_sleep(sleep_time);
	}
}

void	print_burn(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	long			diff_ms;

	gettimeofday(&tv, NULL);
	diff_ms = ((tv.tv_sec - sim->start.tv_sec) * 1000L)
		+ ((tv.tv_usec - sim->start.tv_usec) / 1000L);
	pthread_mutex_lock(&sim->m_print);
	printf("%ld %d burned out\n", diff_ms, cod->id);
	pthread_mutex_unlock(&sim->m_print);
}
