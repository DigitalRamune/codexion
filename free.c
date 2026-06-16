/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 13:10:11 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/16 13:10:13 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    free_tab(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->param->nbr_of_coders)
    {
        free(sim->dongle_tab[i].heap);
        pthread_mutex_destroy(&sim->dongle_tab[i].m_dongle);
        pthread_cond_destroy(&sim->dongle_tab[i].cond_dongle);
        pthread_mutex_destroy(&sim->coders_tab[i].m_comp);
        i++;
    }
    free(sim->coders_tab);
    free(sim->dongle_tab);
    pthread_mutex_destroy(&sim->m_flag);
    pthread_mutex_destroy(&sim->m_print);
    return ;
}