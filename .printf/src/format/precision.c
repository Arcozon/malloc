/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:42:27 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:27:37 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

void	parse_precision(t_mpf *mpf)
{
	if (mpf->format[mpf->i] == '.')
	{
		mpf->flags |= F_PRECI;
		++mpf->i;
		if (mpf->format[mpf->i] == '*')
		{
			mpf->flags |= F_ASTER;
		}
		else
		{
			while (is_digit(mpf->format[mpf->i]))
			{
				mpf->precision = mpf->precision * 10
					+ mpf->format[mpf->i] - '0';
				++mpf->i;
			}
		}
	}
}
