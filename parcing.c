/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:25:43 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/20 14:37:22 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

char	*ft_parcing(int argc)
{
	if (argc < 9)
	{
		return ("Missing arguments");
	}
	return ("Valid numbers of args");
}