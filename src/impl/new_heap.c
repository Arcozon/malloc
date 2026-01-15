#include "impl_mlc.h"

t_arena arenas[3] = {
	[ARENA_TINY] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0},
	[ARENA_SMALL] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0},
	[ARENA_LARGE] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0}
};

t_heap	*_new_heap(const size_t size, t_heap *restrict prev)
{
	t_heap	*ret = mmap(NULL, size, PROT_READ | PROT_WRITE, 
			MAP_PRIVATE | MAP_ANONYMOUS
			, -1, 0);

	if (ret == MAP_FAILED)
		return (NULL);
	
	ret->prev = prev;
	if (prev)
		prev->next = ret;
	ret->next = 0;
	ret->size = size;
	return (ret);
}

__attribute__((always_inline))
static inline size_t	_round_size(size_t size)
{
	const unsigned long map_size = sysconf(_SC_PAGESIZE);

	if ((long)map_size == -1)
		return (0);

	const size_t	rest_size = size % map_size;
	
	if (!rest_size)
		return (size);
	return (size + map_size - rest_size);
}

t_heap	*new_tiny_heap(t_heap *restrict prev)
{
	size_t	tiny_size = _round_size(_M_TINY_SIZE);
	
	if (!tiny_size)
		return (0);
	return (_new_heap(tiny_size, prev));
}
