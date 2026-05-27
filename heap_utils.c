/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 17:11:54 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/27 09:07:31 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
			else if (req1.request_time.tv_usec == req2.request_time.tv_sec)
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
