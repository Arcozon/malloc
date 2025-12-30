/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_ulong_b16_to_buff.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:08 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/15 13:21:39 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

#include "my_printf.h"

int	get_ulong_len_b16(t_uint64 nb)
{
	int	len;

	len = 1;
	while (nb / 16)
	{
		nb /= 16;
		++len;
	}
	return (len);
}

static unsigned long	get_pow16(int pow)
{
	unsigned long	res;

	res = 1;
	while (pow--)
		res *= 16;
	return (res);
}

void	cpy_ulong_b16_to_buff(t_mpf *mpf, t_uint64 nb, char base[])
{
	unsigned long	pow16;

	pow16 = get_pow16(get_ulong_len_b16(nb) - 1);
	while (pow16)
	{
		cpy_to_buff(mpf, base[nb / pow16 % 16]);
		pow16 /= 16;
	}
}
