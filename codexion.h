/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:30:49 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/22 16:46:57 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <bsd/string.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct P_arg
{
	int		nbr_of_coders;
	int		time_to_burn;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		required_compiles;
	int		dongle_coooldown;
	char	*scheduler;
}			t_arg;

typedef struct S_sim
{
	int				stop_flag;
	pthread_mutex_t	m_flag;
	pthread_mutex_t	m_print;
	struct timeval	start;
	struct P_arg	*param;
}					t_sim;


int	ft_parsing(char *data, char **argv);
int	error_return(int z, char *arg_name);
int	valid_data(int *data, int argc, char **argv);
int	fill_struct(int argc, char **argv, struct P_arg *param);

#endif