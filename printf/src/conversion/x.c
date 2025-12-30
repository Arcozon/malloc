/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:53:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/06 12:08:56 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

t_uint32	calc_nb_0_for_x(t_mpf *mpf, t_uint32 *len)
{
	t_uint32	nb_zero;

	nb_zero = 0;
	if (mpf->flags & F_PRECI && mpf->precision > *len)
		nb_zero = mpf->precision - *len;
	if (mpf->flags & F_HASH)
		*len += MPF_OX_LEN;
	if (!(mpf->flags & (F_MINUS | F_PRECI)) && mpf->flags & F_ZERO
		&& mpf->flags & F_WIDTH && mpf->width > *len)
	{
		nb_zero = mpf->width - *len;
		mpf->width = 0;
	}
	return (nb_zero);
}

void	cpy_x(t_mpf *mpf, char *base_16, char *ox)
{
	t_uint32	nb;
	t_uint32	len;
	t_uint32	nb_zero;

	nb = (t_uint32)va_arg(mpf->valst, t_uint32);
	if (!nb)
		mpf->flags &= ~F_HASH;
	len = get_ulong_len_b16(nb);
	nb_zero = calc_nb_0_for_x(mpf, &len);
	len += nb_zero;
	if (nb == 0 && mpf->flags & F_PRECI && !mpf->precision)
		len = 0;
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', len);
	if (mpf->flags & F_HASH)
		cpy_strn_to_buff(mpf, ox, MPF_OX_LEN);
	fill_n_format(mpf, '0', nb_zero);
	if (len)
		cpy_ulong_b16_to_buff(mpf, nb, base_16);
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', len);
}

void	x(t_mpf *mpf)
{
	cpy_x(mpf, MPF_BASE_16_LOWER, MPF_OX_LOWER);
}

void	xx(t_mpf *mpf)
{
	cpy_x(mpf, MPF_BASE_16_UPPER, MPF_OX_UPPER);
}
