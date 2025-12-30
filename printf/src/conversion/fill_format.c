/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_format.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:08:16 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:15:24 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	fill_n_format(t_mpf *mpf, char c, t_uint32 n_to_fill)
{
	while (n_to_fill && !mpf->w_error)
	{
		cpy_to_buff(mpf, c);
		--n_to_fill;
	}
}

void	fill_width(t_mpf *mpf, char c, t_uint32 len_rest)
{
	if (mpf->flags & F_WIDTH && mpf->width > len_rest)
		fill_n_format(mpf, c, mpf->width - len_rest);
}
