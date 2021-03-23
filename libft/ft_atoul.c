/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoul.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 17:03:04 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/20 20:25:56 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long	ft_atoul(const char *str)
{
	unsigned long	num;

	num = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+')
		str++;
	while (ft_isdigit(*str))
		num = num * 10 + *(str++) - '0';
	return num;
}
