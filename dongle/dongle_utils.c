/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 13:09:54 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/23 15:28:39 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

int	is_min(t_dongle *dong, t_coder *cod)
{
	t_request	result;

	result = check_min(dong);
	if (cod->id == result.id)
		return (1);
	return (0);
}

int	cond_check(t_dongle *dong, t_coder *cod)
{
	if (is_min(dong, cod))
	{
		if (!dong->is_used)
			return (1);
	}
	return (0);
}

void	handle_cooldown(t_dongle *dong)
{
	struct timeval	tv;
	struct timeval	c_end;
	long			wait;

	c_end = timeval_add(dong->released_at, dong->cooldown);
	while (1)
	{
		gettimeofday(&tv, NULL);
		wait = ((c_end.tv_sec - tv.tv_sec) * 1000000L)
			+ (c_end.tv_usec - tv.tv_usec);
		if (wait <= 0)
			break ;
		if (wait > 2000)
			precise_sleep(wait / 2);
	}
}

void	dongle_acquisition(t_dongle *dong, t_coder *cod, t_sim *sim)
{
	pthread_mutex_lock(&dong->m_dongle);
	heap_insert(sim, dong, cod);
	while (!cond_check(dong, cod) && !check_stop_flag(sim))
		pthread_cond_wait(&dong->cond_dongle, &dong->m_dongle);
	if (!check_stop_flag(sim))
	{
		heap_extract(sim, dong);
		dong->is_used = 1;
	}
	pthread_mutex_unlock(&dong->m_dongle);
	if (!check_stop_flag(sim))
		handle_cooldown(dong);
}

void	dongle_liberation(t_sim *sim, t_dongle *dong)
{
	struct timeval	tv;

	(void)sim;
	pthread_mutex_lock(&dong->m_dongle);
	dong->is_used = 0;
	gettimeofday(&tv, NULL);
	dong->released_at.tv_sec = tv.tv_sec;
	dong->released_at.tv_usec = tv.tv_usec;
	pthread_cond_broadcast(&dong->cond_dongle);
	pthread_mutex_unlock(&dong->m_dongle);
}
