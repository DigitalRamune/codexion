/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:41:05 by inaciri           #+#    #+#             */
/*   Updated: 2026/04/20 14:41:50 by inaciri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	ft_atoi(const char *str)
{
	char	*tempstr;
	int		neg;
	int		i;
	int		res;

	res = 0;
	tempstr = (char *)str;
	i = 0;
	neg = 1;
	while ((tempstr[i] >= 8 && tempstr[i] <= 13) || tempstr[i] == 32)
		i++;
	if (tempstr[i] == '-' || tempstr[i] == '+')
	{
		if (tempstr[i] == '-')
			neg = neg * -1;
		i++;
	}
	while (tempstr[i] >= '0' && tempstr[i] <= '9')
	{
		res = res * 10 + (tempstr[i] - '0');
		i++;
	}
	return (res * neg);
}
