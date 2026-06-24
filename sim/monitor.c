/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 14:19:31 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/24 14:23:25 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

static void	broadcast_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		pthread_mutex_lock(&sim->dongle_tab[i].m_dongle);
		pthread_cond_broadcast(&sim->dongle_tab[i].cond_dongle);
		pthread_mutex_unlock(&sim->dongle_tab[i].m_dongle);
		i++;
	}
}

static void	trigger_stop(t_sim *sim, int res)
{
	pthread_mutex_lock(&sim->m_flag);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->m_flag);
	if (res == 1)
		broadcast_all(sim);
}

void	*ft_monitor(void *arg)
{
	int		res;
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!check_stop_flag(sim))
	{
		res = check_burn_end(sim);
		if (res == 1 || res == 2)
		{
			trigger_stop(sim, res);
			break ;
		}
		precise_sleep(5000);
	}
	return (NULL);
}
