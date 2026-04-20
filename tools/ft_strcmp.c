/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:30:16 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/20 15:39:26 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	int				minus;

	i = 0;
	minus = 0;
	if (!s1 || !s2)
		return (-1);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (-1);
	while (s1[i] && s2[i])
	{
		minus = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (minus != 0)
			break ;
		i++;
	}
	return (minus);
}