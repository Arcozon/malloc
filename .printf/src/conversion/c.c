/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:51:28 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:29:12 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	c(t_mpf *mpf)
{
	if (!(mpf->flags & F_MINUS))
		fill_width(mpf, ' ', 1);
	cpy_to_buff(mpf, (char)va_arg(mpf->valst, int));
	if (mpf->flags & F_MINUS)
		fill_width(mpf, ' ', 1);
}
