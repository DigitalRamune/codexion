/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:22:41 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 15:23:31 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

void	cooldown_convert(t_dongle *dongle, t_sim *sim)
{
	struct timeval	cooldown_conv;

	cooldown_conv.tv_sec = sim->param->dongle_cooldown / 1000;
	cooldown_conv.tv_usec = (sim->param->dongle_cooldown % 1000) * 1000;
	dongle->cooldown.tv_sec = cooldown_conv.tv_sec;
	dongle->cooldown.tv_usec = cooldown_conv.tv_usec;
}

int	init_dongle_heap(t_dongle *dongle)
{
	dongle->heap = malloc((dongle->max_coders + 1) * sizeof(t_request));
	if (!dongle->heap)
	{
		dongle->heap = NULL;
		return (0);
	}
	return (1);
}

int	init_dongle(t_dongle *dongle, t_sim *sim)
{
	pthread_mutex_init(&dongle->m_dongle, NULL);
	pthread_cond_init(&dongle->cond_dongle, NULL);
	dongle->is_used = 0;
	dongle->max_coders = 2;
	dongle->in_heap = 0;
	dongle->released_at.tv_sec = 0;
	dongle->released_at.tv_usec = 0;
	cooldown_convert(dongle, sim);
	if (!init_dongle_heap(dongle))
		return (0);
	return (1);
}

int	add_all_dongle(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		if (!init_dongle(&sim->dongle_tab[i], sim))
			return (0);
		i++;
	}
	return (1);
}
