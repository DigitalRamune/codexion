/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:02:58 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 15:24:06 by inaciri          ###   ########.fr       */
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
