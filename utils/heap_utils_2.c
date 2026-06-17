/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 14:04:07 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/17 14:13:36 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

void	heap_insert(t_sim *sim, t_dongle *dong, t_coder *cod)
{
	t_request		req;
	struct timeval	tv;
	struct timeval	burn_conv;
	struct timeval	deadline;

	if (dong->in_heap < dong->max_coders)
	{
		gettimeofday(&tv, NULL);
		burn_conv.tv_sec = sim->param->time_to_burn / 1000;
		burn_conv.tv_usec = (sim->param->time_to_burn % 1000) * 1000;
		deadline.tv_sec = cod->last_compile.tv_sec + burn_conv.tv_sec;
		deadline.tv_usec = cod->last_compile.tv_usec + burn_conv.tv_usec;
		if (deadline.tv_usec >= 1000000)
		{
			deadline.tv_usec -= 1000000;
			deadline.tv_sec += 1;
		}
		req.id = cod->id;
		req.request_time.tv_sec = tv.tv_sec;
		req.request_time.tv_usec = tv.tv_usec;
		req.deadline.tv_sec = deadline.tv_sec;
		req.deadline.tv_usec = deadline.tv_usec;
		heap_bubble_up(sim, dong, req);
	}
}

t_request	heap_extract(t_sim *sim, t_dongle *dongle)
{
	t_request	error_req;

	if (dongle->in_heap <= 0)
	{
		error_req.id = -1;
		return (error_req);
	}
	return (heap_bubble_down(sim, dongle));
}

t_request	check_min(t_dongle *dongle)
{
	t_request	error_req;

	if (dongle->in_heap > 0)
		return (dongle->heap[0]);
	error_req.id = -1;
	return (error_req);
}
