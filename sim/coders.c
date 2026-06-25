/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:14:11 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/25 16:33:52 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

void	request_both_dongle(t_sim *sim, t_coder *cod)
{
	if (cod->left == cod->right)
		return ;
	if (cod->left->id > cod->right->id)
	{
		dongle_acquisition(cod->right, cod, sim);
		if (!check_stop_flag(sim))
			print_dong(sim, cod);
		dongle_acquisition(cod->left, cod, sim);
		if (!check_stop_flag(sim))
			print_dong(sim, cod);
	}
	else
	{
		dongle_acquisition(cod->left, cod, sim);
		if (!check_stop_flag(sim))
			print_dong(sim, cod);
		dongle_acquisition(cod->right, cod, sim);
		if (!check_stop_flag(sim))
			print_dong(sim, cod);
	}
}

void	liberate_both_dongle(t_sim *sim, t_coder *cod)
{
	if (cod->left == cod->right)
		return ;
	dongle_liberation(sim, cod->left);
	dongle_liberation(sim, cod->right);
}

void	init_coder(t_coder *cod, t_sim *sim, int id)
{
	pthread_mutex_init(&cod->m_comp, NULL);
	cod->id = id;
	cod->compilations = 0;
	cod->last_compile.tv_sec = sim->start.tv_sec;
	cod->last_compile.tv_usec = sim->start.tv_usec;
	cod->simulation = sim;
	if (id == 0)
	{
		cod->left = &sim->dongle_tab[sim->param->nbr_of_coders - 1];
		cod->right = &sim->dongle_tab[0];
	}
	else
	{
		cod->left = &sim->dongle_tab[id - 1];
		cod->right = &sim->dongle_tab[id];
	}
}

void	add_all_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		init_coder(&sim->coders_tab[i], sim, i);
		i++;
	}
}

void	*cod_main(void *arg)
{
	t_coder	*cod;
	t_sim	*sim;

	cod = (t_coder *)arg;
	sim = cod->simulation;
	wait_until_start(sim);
	if (cod->left == cod->right)
	{
		while (!check_stop_flag(sim))
			precise_sleep(1000);
		return (NULL);
	}
	if (cod->id % 2 != 0)
		precise_sleep(sim->param->time_to_compile * 500L);
	while (cod->compilations < sim->param->required_compiles
		&& !check_stop_flag(sim))
	{
		request_both_dongle(sim, cod);
		print_comp(sim, cod);
		liberate_both_dongle(sim, cod);
		print_debug(sim, cod);
		print_refactor(sim, cod);
		coder_think(sim);
	}
	return (NULL);
}
