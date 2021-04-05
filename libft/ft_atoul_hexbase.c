/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoul_hexbase.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 17:03:04 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/02 13:02:25 by dhasegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long	ft_atoul_hexbase(const char *str)
{
	unsigned long	num;
	const char* 	base = "0123456789abcdef";
	unsigned int	idx;
	char			c;

	num = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+')
		str++;
	while (ft_isdigit(*str) || ('a' <= *str && *str <= 'f')
			|| ('A' <= *str && *str <= 'F'))
	{
		c = ft_tolower(*str);
		idx = -1;
		while (++idx < 16)
			if (c == base[idx])
				break ;
		num = num * 16 + idx;
		str++;
	}
	return num;
}
