/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burn.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:39:25 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/09 18:08:39 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	detect_burn(t_sim *sim, t_coder *cod)
{
	struct timeval	tv;
	struct timeval	burn_conv;
	struct timeval	deadline;

	gettimeofday(&tv, NULL);
	burn_conv.tv_sec = sim->param->time_to_burn / 1000;
	burn_conv.tv_usec = (sim->param->time_to_burn % 1000) * 1000; 
	pthread_mutex_lock(&cod->m_comp);
	deadline = timeval_add(cod->last_compile, burn_conv);
	pthread_mutex_unlock(&cod->m_comp);
	if (!timeval_compare(tv, deadline))
		return (0);
	return (1);
}

int	burn_loop(t_sim *sim, t_coder *cod)
{
	if (!detect_burn(sim, cod))
	{
		pthread_mutex_lock(&sim->m_print);
		print_burn(sim, cod);
		pthread_mutex_unlock(&sim->m_print);
		pthread_mutex_lock(&sim->m_flag);
		sim->stop_flag = 1;
		pthread_mutex_unlock(&sim->m_flag);
		pthread_cond_broadcast(&cod->left->cond_dongle);
		pthread_cond_broadcast(&cod->right->cond_dongle);
		return (0);
	}
	return (1);
}

int	detect_end(t_sim *sim, t_coder *cod)
{
	int	comp_nbr;

	pthread_mutex_lock(&cod->m_comp);
	comp_nbr = cod->compilations;
	pthread_mutex_unlock(&cod->m_comp);
	if (comp_nbr == sim->param->required_compiles)
		return (0);
	return (1);
}

int	check_burn_end(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		if (!burn_loop(sim, &sim->coders_tab[i]))
			return (0);
		if (detect_end(sim, &sim->coders_tab[i]))
			i++;
		else
			return (-1);
	}
	return (1);
}