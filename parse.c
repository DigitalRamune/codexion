/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 18:34:02 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/22 16:21:23 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	argument_names(char **arg_tab)
{
	arg_tab[0] = "number of coders";
	arg_tab[1] = "time to burn";
	arg_tab[2] = "time to compile";
	arg_tab[3] = "time to debug";
	arg_tab[4] = "time to refactor";
	arg_tab[5] = "number of required compilations";
	arg_tab[6] = "dongle cooldown";
	arg_tab[7] = "scheduler";
}

int	error_return(int z, char *arg_name)
{
	if (z == 0)
	{
		printf("Invalid number of arguments given\n");
		return (0);
	}
	else if (z == 1)
	{
		printf("Invalid %s\n", arg_name);
		return (0);
	}
	return (0);
}

int	valid_data(int *data, int argc, char **argv)
{
	int		i;
	char	*arg_name[8];

	argument_names(arg_name);
	i = 0;
	if (argc < 9 || argc > 9)
		return (error_return(0, NULL));
	while (i < 7)
	{
		data[i] = atoi(argv[i + 1]);
		if (data[i] < 0)
			return (error_return(1, arg_name[i]));
		i++;
	}
	if (!strcmp(argv[8], "FIFO") || !strcmp(argv[8], "fifo"))
		data[i] = 0;
	else if (!strcmp(argv[8], "EDF") || !strcmp(argv[8], "edf"))
		data[i] = 1;
	else
	{
		printf("Invalid %s\n", arg_name[i]);
		return (0);
	}
	return (1);
}

int	fill_struct(int argc, char **argv, struct P_arg *param)
{
	int	data[8];

	if (!valid_data(data, argc, argv))
		return (0);
	(*param).nbr_of_coders = data[0];
	(*param).time_to_burn = data[1];
	(*param).time_to_compile = data[2];
	(*param).time_to_debug = data[3];
	(*param).time_to_refactor = data[4];
	(*param).required_compiles = data[5];
	(*param).dongle_coooldown = data[6];
	if (data[7] == 0)
		(*param).scheduler = "fifo";
	else
		(*param).scheduler = "edf";
	return (1);
}
