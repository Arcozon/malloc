/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:34:29 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/17 16:09:05 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

static t_uint8	flag_value(char c)
{
	if (c == '-')
		return (F_MINUS);
	if (c == '0')
		return (F_ZERO);
	if (c == '+')
		return (F_PLUS);
	if (c == '#')
		return (F_HASH);
	if (c == ' ')
		return (F_SPACE);
	return (0);
}

void	parse_flags(t_mpf *mpf)
{
	while (flag_value(mpf->format[mpf->i]))
	{
		mpf->flags |= flag_value(mpf->format[mpf->i]);
		++mpf->i;
	}
	if (mpf->flags & F_MINUS)
		mpf->flags &= ~F_ZERO;
}
