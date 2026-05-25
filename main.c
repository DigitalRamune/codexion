/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 15:09:30 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/25 19:00:42 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	main(int argc, char **argv)
{
	struct P_arg	param;

	if (!fill_struct(argc, argv, &param))
		return (0);
	printf("All param are good\n");
	print_struct(&param);
}
