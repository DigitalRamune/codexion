/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 13:09:54 by inaciri           #+#    #+#             */
/*   Updated: 2026/06/08 13:09:56 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int is_min(t_dongle *dong, t_coder *cod)
{
    t_request   result;

    result = check_min(dong);
    if (cod->id == result.id)
        return (1);
    return (0);
}

int cond_check(t_dongle *dong, t_coder *cod)
{
    struct timeval  tv;
    struct timeval  cooldown;

    cooldown.tv_sec = dong->cooldown.tv_sec + dong->released_at.tv_sec;
    cooldown.tv_usec = dong->cooldown.tv_usec + dong->released_at.tv_usec;
    if (cooldown.tv_usec >= 1000000)
	{
		cooldown.tv_usec -= 1000000;
		cooldown.tv_sec += 1;
	}
    if (is_min(dong, cod))
    {
        if (!dong->is_used)
        {
            gettimeofday(&tv, NULL);
            if (tv.tv_sec > cooldown.tv_sec)
                return (1);
            else if (tv.tv_sec == cooldown.tv_sec)
            {
                if (tv.tv_usec >= cooldown.tv_usec)
                    return (1);
            }
        }
    }
    return (0);
}

void    dongle_acquisition(t_dongle *dong, t_coder *cod, t_sim *sim)
{
    pthread_mutex_lock(&dong->m_dongle);
    heap_insert(sim, dong, cod);
    while(!cond_check(dong, cod) && !sim->stop_flag)
        pthread_cond_wait(&dong->cond_dongle, &dong->m_dongle);
    if (!sim->stop_flag)
    {
        heap_extract(sim, dong);
        dong->is_used = 1;
    }
    pthread_mutex_unlock(&dong->m_dongle);
}

void    dongle_liberation(t_sim *sim, t_dongle *dong)
{
    struct timeval  tv;

    pthread_mutex_lock(&dong->m_dongle);
    dong->is_used = 0;
    gettimeofday(&tv, NULL);
    dong->released_at.tv_sec = tv.tv_sec;
    dong->released_at.tv_usec = tv.tv_usec;
    pthread_cond_broadcast(&dong->cond_dongle);
    pthread_mutex_unlock(&dong->m_dongle);
}