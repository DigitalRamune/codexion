/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edge_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:36:52 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/12 11:44:21 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	one_coder(t_sim *sim)
{
	if (sim->param->nbr_of_coders < 2)
		return (0);
	return (1);
}

int check_stop_flag(t_sim *sim)
{
    int flag;

    pthread_mutex_lock(&sim->m_flag);
    flag = sim->stop_flag;
    pthread_mutex_unlock(&sim->m_flag);
    return (flag);
}
