/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:02:58 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/23 15:28:54 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

struct timeval	timeval_add(struct timeval a, struct timeval b)
{
	struct timeval	result;

	result.tv_sec = a.tv_sec + b.tv_sec;
	result.tv_usec = a.tv_usec + b.tv_usec;
	if (result.tv_usec >= 1000000)
	{
		result.tv_usec -= 1000000;
		result.tv_sec += 1;
	}
	return (result);
}

int	timeval_compare(struct timeval a, struct timeval b)
{
	if (a.tv_sec > b.tv_sec)
		return (1);
	else if (a.tv_sec == b.tv_sec)
	{
		if (a.tv_usec > b.tv_usec)
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

void	precise_sleep(long microsec)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;

	gettimeofday(&start, NULL);
	if (microsec > 2000)
		usleep(microsec - 2000);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000L
			+ (current.tv_usec - start.tv_usec);
		if (elapsed >= microsec)
			break ;
	}
}

void	coder_think(t_sim *sim)
{
	long	think_time;

	if (sim->param->nbr_of_coders % 2 != 0)
	{
		think_time = sim->param->time_to_compile - sim->param->time_to_debug
			- sim->param->time_to_refactor;
		if (think_time < 0)
			think_time = 0;
		precise_sleep((think_time * 1000L) + 5000L);
	}
}
