/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:58:07 by gaeudes           #+#    #+#             */
/*   Updated: 2025/07/05 09:36:14 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_printf.h"

void	reset_flags_mpf(t_mpf *mpf)
{
	mpf->flags = 0;
	mpf->precision = 0;
	mpf->width = 0;
}

void	init_mpf(t_mpf *mpf, const char *format)
{
	mpf->format = format;
	mpf->i = 0;
	mpf->fd = 1;
	mpf->w_error = 0;
	mpf->r_value = 0;
	mpf->b_i = 0;
}

void	routine_printf(t_mpf *mpf)
{
	while (mpf->format[mpf->i] && !mpf->w_error)
	{
		if (mpf->format[mpf->i] == '%')
			print_format(mpf);
		else
		{
			cpy_to_buff(mpf, mpf->format[mpf->i]);
			++mpf->i;
		}
	}
	write_buff(mpf);
}

int	ft_printf(const char *format, ...)
{
	t_mpf	mpf;

	va_start(mpf.valst, format);
	init_mpf(&mpf, format);
	routine_printf(&mpf);
	va_end(mpf.valst);
	if (mpf.w_error)
		return (-1);
	return (mpf.r_value);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	t_mpf	mpf;

	va_start(mpf.valst, format);
	init_mpf(&mpf, format);
	mpf.fd = fd;
	routine_printf(&mpf);
	va_end(mpf.valst);
	if (mpf.w_error)
		return (-1);
	return (mpf.r_value);
}
