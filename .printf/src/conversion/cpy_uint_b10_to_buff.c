/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_uint_b10_to_buff.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:28 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 10:57:38 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

int	get_int_len_b10(t_int32 nb)
{
	int	len;

	len = 1;
	while (nb / 10)
	{
		nb /= 10;
		++len;
	}
	return (len);
}

int	get_uint_len_b10(t_uint32 nb)
{
	int	len;

	len = 1;
	while (nb / 10)
	{
		nb /= 10;
		++len;
	}
	return (len);
}

static unsigned int	get_pow10(int pow)
{
	unsigned int	res;

	res = 1;
	while (pow--)
		res *= 10;
	return (res);
}

void	cpy_uint_b10_to_buff(t_mpf *mpf, unsigned int nb)
{
	unsigned int	pow10;

	pow10 = get_pow10(get_uint_len_b10(nb) - 1);
	while (pow10)
	{
		cpy_to_buff(mpf, MPF_BASE_10[nb / pow10 % 10]);
		pow10 /= 10;
	}
}
