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

#ifndef IMPL_MLC_H
# define IMPL_MLC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdint.h>
# include <pthread.h>

# include "ft_printf.h"
# include "libft_malloc.h"

# ifndef NULL
#  define NULL	((void *)0)
# endif

# define _M_BIT_ALIGN	4UL
# define _M_ALIGN	(1UL << _M_BIT_ALIGN)
# define _M_ALIGN_MASK	(_M_ALIGN - 1UL)

# define _M_TINY_MAX_ALC_SIZE	31UL
# define _M_SMALL_MIN_ALC_SIZE	(_M_TINY_MAX_ALC_SIZE + 1UL)
# define _M_SMALL_MAX_ALC_SIZE	127UL
# define _M_LARGE_MIN_ALC_SIZE	(_M_SMALL_MAX_ALC_SIZE + 1UL)

# define _M_TINY_SIZE	(sizeof(t_heap) + 100UL * (sizeof(t_chunk) + _M_TINY_MAX_SIZE))
# define _M_SMALL_SIZE	(sizeof(t_heap) + 100UL * (sizeof(t_chunk) + _M_SMALL_MAX_SIZE))

# define _M_FREE_MASK	04UL
# define _M_AREN_MASK	03UL

enum
{
	ARENA_TINY = 0UL,
#	define ARENA_TINY	ARENA_TINY
	ARENA_SMALL = 1UL,
#	define ARENA_SMALL	ARENA_SMALL
	ARENA_LARGE = 2UL,
#	define ARENA_LARGE	ARENA_LARGE
};

typedef struct s_arena	t_arena;
typedef struct s_heap	t_heap;
typedef struct s_chunck	t_chunck;

struct s_heap	{
	t_heap	*next;
	t_heap	*prev;
	size_t	size;
	# if ( ((sizeof(s_heap *) + sizeof(size_t)) % _M_ALIGN) != 0)
		uint8_t	_padding[((sizeof(s_heap *) + sizeof(s_heap *) + sizeof(size_t)) % _M_ALIGN)];	
	# endif
};

struct s_arena	{
	pthread_mutex_t	mtx;	
	t_heap		*heap;
};

struct s_chunck
{
	t_chunk	*bck;
	size_t	size;
	# if ( ((sizeof(t_chunk *) + sizeof(size_t)) % _M_ALIGN) != 0)
		uint8_t	_padding[((sizeof(t_chunk *) + sizeof(size_t)) % _M_ALIGN)];
	# endif
};

extern t_arena	arenas[3]; 

#endif
