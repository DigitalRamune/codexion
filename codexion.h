/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:30:49 by inaciri           #+#    #+#             */
/*   Updated: 2026/05/29 15:38:23 by inaciri          ###   ########.fr       */
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
# include <sys/time.h>
# include <pthread.h>

typedef struct P_arg
{
	int		nbr_of_coders;
	int		time_to_burn;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		required_compiles;
	int		dongle_cooldown;
	char	*scheduler;
}			t_arg;

typedef struct S_sim
{
	int				stop_flag;
	pthread_mutex_t	m_flag;
	pthread_mutex_t	m_print;
	struct timeval	start;
	struct D_dongle	*dongle_tab;
	struct C_coder	*coders_tab;
	t_arg			*param;
}					t_sim;

typedef struct R_request
{
	int				id;
	struct timeval	request_time;
	struct timeval	deadline;
}					t_request;

typedef struct D_dongle
{
	pthread_mutex_t		m_dongle;
	pthread_cond_t		cond_dongle;
	int					is_used;
	t_request			*heap;
	int					in_heap;
	int					max_coders;
	struct timeval		cooldown;
}						t_dongle;

typedef struct C_coder
{
	int				id;
	int				compilations;
	t_dongle		*left;
	t_dongle		*right;
	struct timeval	last_compile;
	t_sim			*simulation;
}					t_coder;

int			ft_parsing(char *data, char **argv);
int			error_return(int z, char *arg_name);
int			valid_data(int *data, int argc, char **argv);
int			fill_struct(int argc, char **argv, struct P_arg *param);

int			ft_edf_compare(t_request req1, t_request req2);
int			ft_compare(t_sim *simulation, t_request req1, t_request req2);
void		heap_bubble_up(t_sim *simulation, t_dongle *dongle, t_request new_req);
void		ft_replace(t_request save, t_request replacing, t_request new_one);
t_request	heap_bubble_down(t_sim *simulation, t_dongle *dongle);

#endif