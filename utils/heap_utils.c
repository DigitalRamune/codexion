/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 17:11:54 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/17 14:12:34 by inaciri          ###   ########.fr       */
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
	while (ft_compare(simulation, dongle->heap[parent_index],
			dongle->heap[kid_index]) && kid_index != 0)
	{
		temp = dongle->heap[parent_index];
		dongle->heap[parent_index] = dongle->heap[kid_index];
		dongle->heap[kid_index] = temp;
		kid_index = parent_index;
		parent_index = (kid_index - 1) / 2;
	}
	return ;
}

static int	get_swap_child(t_sim *sim, t_dongle *dongle, int current)
{
	int	c1;
	int	c2;
	int	smallest;

	c1 = (2 * current) + 1;
	c2 = (2 * current) + 2;
	if (c1 >= dongle->in_heap)
		return (-1);
	if (c2 >= dongle->in_heap)
		smallest = c1;
	else if (ft_compare(sim, dongle->heap[c1], dongle->heap[c2]))
		smallest = c2;
	else
		smallest = c1;
	if (ft_compare(sim, dongle->heap[current], dongle->heap[smallest]))
		return (smallest);
	return (-1);
}

t_request	heap_bubble_down(t_sim *sim, t_dongle *dongle)
{
	t_request	save;
	t_request	temp;
	int			current;
	int			next;

	save = dongle->heap[0];
	dongle->heap[0] = dongle->heap[dongle->in_heap - 1];
	dongle->in_heap -= 1;
	current = 0;
	next = get_swap_child(sim, dongle, current);
	while (next != -1)
	{
		temp = dongle->heap[current];
		dongle->heap[current] = dongle->heap[next];
		dongle->heap[next] = temp;
		current = next;
		next = get_swap_child(sim, dongle, current);
	}
	return (save);
}
