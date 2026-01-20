#include "ft_printf.h"
#include "impl_mlc.h"

t_arena arenas[3] = {
	[ARENA_TINY] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0},
	[ARENA_SMALL] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0},
	[ARENA_LARGE] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = 0}
};

t_heap	*_new_heap(const size_t _size)
{
	t_heap	*nheap = mmap(NULL, _size, PROT_READ | PROT_WRITE, 
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	t_flst	*nflst = (void *)nheap + sizeof(*nheap);

	if (nheap == MAP_FAILED)
		return (NULL);
	nheap->fwd = NULL;
	nheap->flst = nflst;
	nflst->bck = NULL;
	nflst->fwd = NULL;
	nflst->pheap = nheap;
	nflst->size = _size - sizeof(*nheap) - sizeof(t_chunk);
	ft_fprintf(2, " -- New Heap: %p | size: %u\n", nheap, (unsigned int)_size);
	ft_fprintf(2, " -- New Flst: %p | size: %u\n", nflst, (unsigned int)nflst->size);
	return (nheap);
}

__attribute__((always_inline))
static inline size_t	_round_page_size(size_t size)
{
	const unsigned long map_size = sysconf(_SC_PAGESIZE);

	if ((long)map_size == -1)
		return (0);

	const size_t	rest_size = size % map_size;
	
	if (!rest_size && size)
		return (size);
	return (size + map_size - rest_size);
}

t_heap	*new_tiny_heap(t_heap **restrict _pheap)
{
	size_t	tiny_size = _round_page_size(_M_TINY_SIZE);
	
	if (!tiny_size)
		return (NULL);
	while (*_pheap)
		_pheap = &(*_pheap)->fwd;
	*_pheap = _new_heap(tiny_size);
	return (*_pheap);
}

t_heap	*new_small_heap(t_heap **restrict _pheap)
{
	size_t	small_size = _round_page_size(_M_SMALL_SIZE);
	
	if (!small_size)
		return (NULL);
	while (*_pheap)
		_pheap = &(*_pheap)->fwd;
	*_pheap = _new_heap(small_size);
	return (*_pheap);
}

t_heap	*new_heap(t_heap **restrict _pheap, const size_t _alloc_size)
{
	if (_alloc_size <= _M_TINY_MAX_ALC_SIZE) {
		return (new_tiny_heap(_pheap));
	}
	else if (_alloc_size <= _M_SMALL_MAX_ALC_SIZE) {
		return (new_small_heap(_pheap));
	}
	else {
		// BIG PAGE
		return (NULL);
	}
}
