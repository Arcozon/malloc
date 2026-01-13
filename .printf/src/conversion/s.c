/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:53:05 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 12:36:25 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

static t_uint32	ft_strlen(char *s)
{
	t_int32	len;

	len = 0;
	while (s[len])
		++len;
	return (len);
}

void	mprintf_null(t_mpf *mpf)
{
	static char	null[] = "(null)";
	t_uint32	len;

	len = ft_strlen(null);
	if (mpf->flags & F_PRECI && mpf->precision < len)
		len = 0;
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', len);
	cpy_strn_to_buff(mpf, null, len);
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', len);
}

void	s(t_mpf *mpf)
{
	char		*str;
	t_uint32	len;

	str = (char *)va_arg(mpf->valst, char *);
	if (!str)
		mprintf_null(mpf);
	else
	{
		len = ft_strlen(str);
		if (mpf->flags & F_PRECI && mpf->precision < len)
			len = mpf->precision;
		if (!(mpf->flags & F_MINUS))
			fill_width(mpf, ' ', len);
		cpy_strn_to_buff(mpf, str, len);
		if (mpf->flags & F_MINUS)
			fill_width(mpf, ' ', len);
	}
}
