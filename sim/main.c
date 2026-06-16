/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 15:09:30 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 15:23:40 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/codexion.h"

void	print_struct(struct P_arg *param)
{
	printf("%d\n", (*param).nbr_of_coders);
	printf("%d\n", (*param).time_to_burn);
	printf("%d\n", (*param).time_to_compile);
	printf("%d\n", (*param).time_to_debug);
	printf("%d\n", (*param).time_to_refactor);
	printf("%d\n", (*param).required_compiles);
	printf("%d\n", (*param).dongle_cooldown);
	printf("%s\n", (*param).scheduler);
}

void	ft_codexion(t_sim *sim)
{
	pthread_t	*thread_cod;
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	thread_cod = malloc((sim->param->nbr_of_coders + 1) * sizeof(pthread_t));
	if (!thread_cod)
	{
		thread_cod = NULL;
		return ;
	}
	add_all_coders(sim);
	while (i < sim->param->nbr_of_coders)
	{
		pthread_create(&thread_cod[i], NULL, &cod_main, &sim->coders_tab[i]);
		i++;
	}
	pthread_create(&monitor_thread, NULL, &ft_monitor, sim);
	i = 0;
	while (i < sim->param->nbr_of_coders)
	{
		pthread_join(thread_cod[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	free_tab(sim);
	free(thread_cod);
}

int	main(int argc, char **argv)
{
	t_arg	param;
	t_sim	sim;

	if (!fill_struct(argc, argv, &param))
		return (0);
	if (!init_simulation(&sim, &param))
		return (0);
	if (!add_all_dongle(&sim))
		return (0);
	ft_codexion(&sim);
	return (0);
}
