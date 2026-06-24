/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:01:11 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/24 13:22:44 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

int	init_dongle_tab(t_sim *sim)
{
	sim->dongle_tab = malloc((sim->param->nbr_of_coders + 1)
			* sizeof(t_dongle));
	if (!sim->dongle_tab)
	{
		sim->dongle_tab = NULL;
		return (0);
	}
	return (1);
}

int	init_coders_tab(t_sim *sim)
{
	sim->coders_tab = malloc((sim->param->nbr_of_coders + 1) * sizeof(t_coder));
	if (!sim->coders_tab)
	{
		sim->coders_tab = NULL;
		return (0);
	}
	return (1);
}

int	init_simulation(t_sim *sim, t_arg *arg)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	sim->stop_flag = 0;
	pthread_mutex_init(&sim->m_flag, NULL);
	pthread_mutex_init(&sim->m_print, NULL);
	sim->start.tv_sec = tv.tv_sec;
	sim->start.tv_usec = tv.tv_usec;
	sim->param = arg;
	sim->start_sync = 0;
	if (!init_dongle_tab(sim) || !init_coders_tab(sim))
		return (0);
	return (1);
}
