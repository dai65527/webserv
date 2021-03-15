/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 10:24:29 by dnakano           #+#    #+#             */
/*   Updated: 2021/03/16 00:12:32 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C" {
#include "libft.h"
}

bool isAlpha(int n) {
	return ft_isalpha(n);
}

bool isOdd(int n) {
	return (n % 2);
}

bool isEven(int n) {
	return !(n % 2);
}
