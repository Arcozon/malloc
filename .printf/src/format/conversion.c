/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:34:23 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/13 14:48:34 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	parse_format(t_mpf *mpf)
{
	reset_flags_mpf(mpf);
	++mpf->i;
	parse_flags(mpf);
	parse_width(mpf);
	parse_precision(mpf);
}

void	print_format(t_mpf *mpf)
{
	static char	conv[] = "diuxXpcs%";
	static void	(*fct_conv[])(t_mpf *) = {di, di, u, x, xx, p, c, s, percent};
	int			i;

	parse_format(mpf);
	i = 0;
	while (conv[i])
	{
		if (conv[i] == mpf->format[mpf->i])
		{
			fct_conv[i](mpf);
			break ;
		}
		++i;
	}
	++mpf->i;
}
