/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   impl_mlc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 18:10:14 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/28 18:10:39 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../printf/ft_printf.h"

#ifndef IMPL_MLC_H
# define IMPL_MLC_H

# include "libft_malloc.h"

# include <sys/mman.h>
# include <unistd.h>
# include <stdint.h>

# define _MLC_ALIGN		16
# define _MASK_MLC_ALIGN	(_MLC_ALIGN - 1)

# define _MIN_ALC_PER_ZONE	100

# define _TINY_MAX_ALC_SIZE	64
# define _SMALL_MAX_ALC_SIZE	512

# define _TINY_ZONE_SIZE	(sizeof(t_zone) + (_MIN_ALC_PER_ZONE * (_TINY_MAX_ALC_SIZE + sizeof(t_header))))
# define _SMALL_ZONE_SIZE	(sizeof(t_zone) + (_MIN_ALC_PER_ZONE * (_SMALL_MAX_ALC_SIZE + sizeof(t_header))))

enum e_zone
{
	_MLC_ZONE_TINY,
	_MLC_ZONE_SMALL,
	_MLC_ZONE_LARGE
};

typedef struct s_mlc_header t_header;
struct s_mlc_header
{
	size_t	size;
	size_t	isFree;
};

typedef struct s_mlc_zone	t_mlc_zone;
typedef struct s_mlc_zone	t_zone;
struct s_mlc_zone
{
	t_zone	*next;
	void	*end;
};

typedef t_mlc_zone 	t_zone_tiny;
typedef t_mlc_zone	t_zone_small;
typedef struct s_zone_large	t_zone_large;
struct s_zone_large
{
	void	*next;
	size_t	used;
	size_t	size;
};

typedef struct s_zones	t_zones;
struct s_zones
{
	t_zone_tiny	tiny;
	t_zone_small	small;
	t_zone_large	large;
};

t_zone	*_new_zone(const size_t _size);
t_zone	*_new_tiny_zone(void);
t_zone	*_new_small_zone(void);
t_zone	*_new_ezone(const enum e_zone _eZone);

#endif
