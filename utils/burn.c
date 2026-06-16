/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burn.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:39:25 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 15:23:46 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

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
		return (1);
	return (0);
}

int	burn_loop(t_sim *sim, t_coder *cod)
{
	int	i;

	i = 0;
	if (!detect_burn(sim, cod))
	{
		print_burn(sim, cod);
		pthread_mutex_lock(&sim->m_flag);
		sim->stop_flag = 1;
		pthread_mutex_unlock(&sim->m_flag);
		while (i < sim->param->nbr_of_coders)
		{
			pthread_cond_broadcast(&sim->dongle_tab[i].cond_dongle);
			i++;
		}
		return (1);
	}
	return (0);
}

int	detect_end(t_sim *sim, t_coder *cod)
{
	int	comp_nbr;

	pthread_mutex_lock(&cod->m_comp);
	comp_nbr = cod->compilations;
	pthread_mutex_unlock(&cod->m_comp);
	if (comp_nbr >= sim->param->required_compiles)
		return (1);
	return (0);
}

int	check_burn_end(t_sim *sim)
{
	int	i;
	int	burn;
	int	reached_max_compile;

	reached_max_compile = 0;
	burn = 0;
	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		if (burn == 1)
			break ;
		if (detect_end(sim, &sim->coders_tab[i]))
			reached_max_compile += 1;
		else if (burn_loop(sim, &sim->coders_tab[i]))
			burn = 1;
		i++;
	}
	if (reached_max_compile == sim->param->nbr_of_coders)
		return (1);
	if (burn == 1)
		return (2);
	return (0);
}

void	*ft_monitor(void *arg)
{
	int		burn_result;
	int		i;
	t_sim	*sim;

	sim = (t_sim *)arg;
	i = 0;
	while (!check_stop_flag(sim))
	{
		burn_result = check_burn_end(sim);
		if (burn_result == 1 || burn_result == 2)
		{
			pthread_mutex_lock(&sim->m_flag);
			sim->stop_flag = 1;
			pthread_mutex_unlock(&sim->m_flag);
			if (burn_result == 1)
			{
				while (i < sim->param->nbr_of_coders)
				{
					pthread_cond_broadcast(&sim->dongle_tab[i].cond_dongle);
					i++;
				}
			}
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
