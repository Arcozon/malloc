/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libmlc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 17:16:46 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/28 17:19:31 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# ifndef NULL
#  define NULL ( (void *)0 )
# endif

typedef unsigned long size_t;


void	free(void *ptr)	__attribute__((visibility("default")));
void	*malloc(size_t size)	__attribute__((visibility("default")));
void	*realloc(void *ptr, size_t size)	__attribute__((visibility("default")));

void	show_alloc_mem(void)	__attribute__((visibility("default")));

void	dump_heap_ptr(const void *_ptr)	__attribute__((visibility("default")));
void	fdump_heap_ptr(const int _fd, const void *_ptr)	__attribute__((visibility("default")));

void	dump_large_heap(void)	__attribute__((visibility("default")));
void	fdump_large_heap(const int _fd)	__attribute__((visibility("default")));

#endif
