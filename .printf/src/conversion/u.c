/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:53:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:10:48 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

t_uint32	calc_nb_0_for_u(t_mpf *mpf, t_uint32 *len, t_uint32 nb)
{
	t_uint32	nb_zero;

	nb_zero = 0;
	if (mpf->flags & F_PRECI)
	{
		if (!nb && !mpf->precision)
			*len = 0;
		else if (mpf->precision > *len)
			nb_zero = mpf->precision - *len;
	}
	if (!(mpf->flags & (F_MINUS | F_PRECI)) && mpf->flags & F_ZERO
		&& mpf->flags & F_WIDTH && mpf->width > *len)
	{
		nb_zero = mpf->width - *len;
		mpf->width = 0;
	}
	return (nb_zero);
}

void	u(t_mpf *mpf)
{
	t_uint32	nb;
	t_uint32	len;
	t_uint32	nbzero;

	nb = (t_uint32)va_arg(mpf->valst, t_uint32);
	if (mpf->flags & F_MINUS)
		mpf->flags &= ~F_ZERO;
	len = get_uint_len_b10(nb);
	nbzero = calc_nb_0_for_u(mpf, &len, nb);
	len += nbzero;
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', len);
	fill_n_format(mpf, '0', nbzero);
	if (len)
		cpy_uint_b10_to_buff(mpf, nb);
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', len);
}
