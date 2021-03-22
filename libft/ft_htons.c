/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_htons.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 15:23:31 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/19 15:36:00 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>

static int	is_little_endian(void)
{
	uint16_t	n;
	uint8_t		*uc;

	n = 1;
	uc = (uint8_t*)&n;
	return (*uc);
}

uint16_t	ft_htons(uint16_t shortshort)
{
	if (is_little_endian())
		return (shortshort << 8 | shortshort >> 8);
	return (shortshort);
}
