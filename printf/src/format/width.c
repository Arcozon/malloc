/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:34:02 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:15:46 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

void	parse_width(t_mpf *mpf)
{
	if (is_digit(mpf->format[mpf->i]))
		mpf->flags |= F_WIDTH;
	while (is_digit(mpf->format[mpf->i]))
	{
		mpf->width = mpf->width * 10 + mpf->format[mpf->i] - '0';
		++mpf->i;
	}
}
