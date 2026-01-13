/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_buff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:58:42 by gaeudes           #+#    #+#             */
/*   Updated: 2025/05/02 11:24:04 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

int	write_buff(t_mpf *mpf)
{
	if (write(mpf->fd, mpf->buff, mpf->b_i) != mpf->b_i)
		mpf->w_error = 1;
	mpf->r_value += mpf->b_i;
	mpf->b_i = 0;
	return (mpf->w_error);
}

int	cpy_to_buff(t_mpf *mpf, char c)
{
	if (mpf->w_error)
		return (1);
	mpf->buff[mpf->b_i] = c;
	++mpf->b_i;
	if (mpf->b_i == MPF_BUFF_SIZE)
		return (write_buff(mpf));
	return (0);
}

int	cpy_strn_to_buff(t_mpf *mpf, char *str, t_uint32 len)
{
	t_uint32	i;

	i = 0;
	while (str[i] && i < len && !mpf->w_error)
	{
		cpy_to_buff(mpf, str[i]);
		++i;
	}
	return (mpf->w_error);
}
