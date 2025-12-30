/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   di.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:52:29 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 10:58:15 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	cpy_sign_di(t_mpf *mpf, t_int32 nb)
{
	if (nb < 0)
		cpy_to_buff(mpf, '-');
	else if (mpf->flags & F_PLUS)
		cpy_to_buff(mpf, '+');
	else if (mpf->flags & F_SPACE)
		cpy_to_buff(mpf, ' ');
}

void	fix_flag_di(t_mpf *mpf, t_int32 nb)
{
	if (mpf->flags & (F_MINUS | F_PRECI))
		mpf->flags &= ~F_ZERO;
	if (nb < 0)
		mpf->flags &= ~F_PLUS;
	if (mpf->flags & F_PLUS || nb < 0)
		mpf->flags &= ~F_SPACE;
}

t_uint32	calc_nb_0_for_di(t_mpf *mpf, t_uint32 *len, t_int32 nb)
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
	if (mpf->flags & F_PLUS || nb < 0 || mpf->flags & F_SPACE)
		*len += 1;
	if (!(mpf->flags & (F_MINUS | F_PRECI)) && mpf->flags & F_ZERO
		&& mpf->flags & F_WIDTH && mpf->width > *len)
	{
		nb_zero = mpf->width - *len;
		mpf->width = 0;
	}
	return (nb_zero);
}

void	di(t_mpf *mpf)
{
	t_int32		nb;
	t_uint32	len;
	t_uint32	nb_zero;

	nb = (t_uint32)va_arg(mpf->valst, t_uint32);
	fix_flag_di(mpf, nb);
	len = get_int_len_b10(nb);
	nb_zero = calc_nb_0_for_di(mpf, &len, nb);
	len += nb_zero;
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', len);
	cpy_sign_di(mpf, nb);
	fill_n_format(mpf, '0', nb_zero);
	if (!(!nb && (mpf->flags & F_PRECI && !mpf->precision)))
	{
		if (nb < 0)
			cpy_uint_b10_to_buff(mpf, -nb);
		else
			cpy_uint_b10_to_buff(mpf, nb);
	}
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', len);
}
