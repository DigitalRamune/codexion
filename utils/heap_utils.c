/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 17:11:54 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 15:23:53 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

int	ft_edf_compare(t_request req1, t_request req2)
{
	if (req1.deadline.tv_sec > req2.deadline.tv_sec)
		return (1);
	else if (req1.deadline.tv_sec == req2.deadline.tv_sec)
	{
		if (req1.deadline.tv_usec > req2.deadline.tv_usec)
			return (1);
		else if (req1.deadline.tv_usec == req2.deadline.tv_usec)
		{
			if (req1.id > req2.id)
				return (1);
			else
				return (0);
		}
		else
			return (0);
	}
	else
		return (0);
}

int	ft_compare(t_sim *simulation, t_request req1, t_request req2)
{
	if (!strcmp(simulation->param->scheduler, "fifo"))
	{
		if (req1.request_time.tv_sec > req2.request_time.tv_sec)
			return (1);
		else if (req1.request_time.tv_sec == req2.request_time.tv_sec)
		{
			if (req1.request_time.tv_usec > req2.request_time.tv_usec)
				return (1);
			else if (req1.request_time.tv_usec == req2.request_time.tv_usec)
			{
				if (req1.id > req2.id)
					return (1);
				else
					return (0);
			}
			else
				return (0);
		}
		else
			return (0);
	}
	else
		return (ft_edf_compare(req1, req2));
}

void	heap_bubble_up(t_sim *simulation, t_dongle *dongle, t_request new_req)
{
	int			parent_index;
	int			kid_index;
	t_request	temp;

	if (dongle->in_heap >= dongle->max_coders)
		return ;
	kid_index = dongle->in_heap;
	dongle->heap[dongle->in_heap] = new_req;
	dongle->in_heap += 1;
	parent_index = (kid_index - 1) / 2;
	while (ft_compare(simulation, dongle->heap[parent_index], dongle->heap[kid_index]) && kid_index != 0)
	{
		temp = dongle->heap[parent_index];
		dongle->heap[parent_index] = dongle->heap[kid_index];
		dongle->heap[kid_index] = temp;
		kid_index = parent_index;
		parent_index = (kid_index - 1) / 2;
	}
	return ;
}

t_request	heap_bubble_down(t_sim *simulation, t_dongle *dongle)
{
	t_request	save;
	t_request	temp;
	int			child1;
	int			child2;
	int			current_index;
	int			stop;

	save = dongle->heap[0];
	dongle->heap[0] = dongle->heap[dongle->in_heap - 1];
	dongle->in_heap -= 1;
	current_index = 0;
	stop = 0;
	while (!stop)
	{
		child1 = (2 * current_index) + 1;
		child2 = (2 * current_index) + 2;
		if (child1 >= dongle->in_heap || child2 >= dongle->in_heap)
		{
			if (child1 < dongle->in_heap)
			{
				if (ft_compare(simulation, dongle->heap[current_index], dongle->heap[child1]))
				{
					temp = dongle->heap[current_index];
					dongle->heap[current_index] = dongle->heap[child1];
					dongle->heap[child1] = temp;
					current_index = child1;
				}
			}
			stop = 1;
			break ;
		}
		if (ft_compare(simulation, dongle->heap[child1], dongle->heap[child2]))
		{
			if (ft_compare(simulation, dongle->heap[current_index], dongle->heap[child2]))
			{
				temp = dongle->heap[current_index];
				dongle->heap[current_index] = dongle->heap[child2];
				dongle->heap[child2] = temp;
				current_index = child2;
			}
			else
				stop = 1;
		}
		else
		{
			if (ft_compare(simulation, dongle->heap[current_index], dongle->heap[child1]))
			{
				temp = dongle->heap[current_index];
				dongle->heap[current_index] = dongle->heap[child1];
				dongle->heap[child1] = temp;
				current_index = child1;
			}
			else
				stop = 1;
		}
	}
	return (save);
}

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
