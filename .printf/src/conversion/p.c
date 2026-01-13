/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:52:38 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:14:44 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	mprintf_nil(t_mpf *mpf)
{
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', MPF_NIL_LEN);
	cpy_strn_to_buff(mpf, MPF_NIL, MPF_NIL_LEN);
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', MPF_NIL_LEN);
}

void	p(t_mpf *mpf)
{
	t_uint64	ptr;
	t_uint32	len;

	ptr = (t_uint64)va_arg(mpf->valst, void *);
	if (!ptr)
		mprintf_nil(mpf);
	else
	{
		len = get_ulong_len_b16(ptr) + MPF_OX_LEN;
		if (!(mpf->flags & F_MINUS))
			fill_width(mpf, ' ', len);
		cpy_strn_to_buff(mpf, MPF_OX_LOWER, MPF_OX_LEN);
		cpy_ulong_b16_to_buff(mpf, ptr, MPF_BASE_16_LOWER);
		if (mpf->flags & F_MINUS)
			fill_width(mpf, ' ', len);
	}
}
