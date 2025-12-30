/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:34 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 18:22:05 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_PRINTF_H
# define MY_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

# include "my_flags.h"
# include "my_types.h"
# include "../ft_printf.h"

// # include "ft_printf.h"

# define MPF_BUFF_SIZE 1024

# define MPF_FLAGS "-0# +"

# define MPF_NIL "(nil)"
# define MPF_NIL_LEN 5

# define MPF_BASE_10 "0123456789"
# define MPF_BASE_16_LOWER "0123456789abcdef"
# define MPF_BASE_16_UPPER "0123456789ABCDEF"
# define MPF_OX_LOWER "0x"
# define MPF_OX_UPPER "0X"
# define MPF_OX_LEN 2

typedef struct s_m_pt
{
	const char	*format;
	size_t		i;

	va_list		valst;
	char		buff[MPF_BUFF_SIZE];
	int			b_i;

	t_uint8		flags;
	t_uint32	width;
	t_uint32	precision;

	int			fd;

	int			w_error:1;
	int			r_value;
}	t_mpf;

int		cpy_to_buff(t_mpf *mpf, char c);
int		cpy_strn_to_buff(t_mpf *mpf, char *str, t_uint32 len);
int		write_buff(t_mpf *mpf);

void	fill_n_format(t_mpf *mpf, char c, t_uint32 n_to_fill);
void	fill_width(t_mpf *mpf, char c, t_uint32 len_rest);

int		get_int_len_b10(t_int32 nb);
int		get_uint_len_b10(t_uint32 nb);
int		get_ulong_len_b16(t_uint64 nb);
void	cpy_uint_b10_to_buff(t_mpf *mpf, unsigned int nb);
void	cpy_ulong_b16_to_buff(t_mpf *mpf, t_uint64 nb, char base[]);

// t_uint32	calc_nb_0_for_x(t_mpf *mpf, t_uint32 *len);

void	reset_flags_mpf(t_mpf *mpf);

void	print_format(t_mpf *mpf);
void	parse_flags(t_mpf *mpf);
void	parse_width(t_mpf *mpf);
void	parse_precision(t_mpf *mpf);

void	u(t_mpf *mpf);
void	xx(t_mpf *mpf);
void	x(t_mpf *mpf);
void	di(t_mpf *mpf);
void	p(t_mpf *mpf);
void	percent(t_mpf *mpf);
void	c(t_mpf *mpf);
void	s(t_mpf *mpf);

#endif

// conversion: cspdiuxX%
// flags: '-0# +'

// ’-0.’ ans width
// ’# +’
// %[flags][width][.precision]conversion;
// # : X x
// . > 0 %d %i %x %X
// - > 0
// . : %s presision max char

// precision : i d u x X s

// i d u x X : default precision is 1

// i d preci > len_nb fill space / 0 if 0f
