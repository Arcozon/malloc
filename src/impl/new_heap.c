#include "ft_printf.h"
#include "impl_mlc.h"
#include <sys/mman.h>

t_arena arenas[3] = {
	[ARENA_TINY] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = NULL},
	[ARENA_SMALL] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = NULL},
	[ARENA_LARGE] = {.mtx = PTHREAD_MUTEX_INITIALIZER, .heap = NULL}
};

t_heap	*_new_heap(const size_t _size, const size_t _flagArena)
{
	t_heap	*newHeap = mmap(NULL, _size, PROT_READ | PROT_WRITE, 
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	t_flst	*nflst = (void *)newHeap + sizeof(*newHeap);

	if (newHeap == MAP_FAILED)
		return (NULL);
	newHeap->fwd = NULL;
	newHeap->bck = NULL;
	newHeap->size = (_size - sizeof(*newHeap)) | _flagArena;
	newHeap->flst = nflst;
	nflst->bck = NULL;
	nflst->fwd = NULL;
	nflst->pheap = newHeap;
	nflst->size = _size - sizeof(*newHeap) - sizeof(t_chunk);
	nflst->size |= _M_FREE_MASK | _flagArena;
//	ft_fprintf(2, " -- New Heap: %p | size: %u\n", newHeap, (unsigned int)_size);
//	ft_fprintf(2, "Bck : %p | Fwd: %p\n", newHeap->bck, newHeap->fwd);
//	ft_fprintf(2, " -- New Flst: %p | size: %u\n\n", nflst, (unsigned int)(nflst->size & ~_M_DATA_MASK));
	return (newHeap);
}

__attribute__((always_inline))
static inline size_t	_round_page_size(size_t size)
{
	const unsigned long map_size = sysconf(_SC_PAGESIZE);

	if ((long)map_size == -1)
		return (0);

	const size_t	rest_size = size % map_size;
	
	if (rest_size == 0 && size)
		return (size);
	return (size + map_size - rest_size);
}

t_heap	*new_tiny_heap(void)
{
	const size_t	tiny_size = _round_page_size(_M_TINY_SIZE);
	
	if (!tiny_size)
		return (NULL);
	return (_new_heap(tiny_size, ARENA_TINY));
}

t_heap	*new_small_heap(void)
{
	const size_t	small_size = _round_page_size(_M_SMALL_SIZE);
	
	if (!small_size)
		return (NULL);
	return (_new_heap(small_size, ARENA_SMALL));
}

t_heap	*new_large_heap(const size_t _mSize)
{
	const size_t	size = _round_page_size(_mSize);
	t_heap		*nlHeap = mmap(NULL, size, PROT_READ | PROT_WRITE, 
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (nlHeap == MAP_FAILED)
		return (NULL);
	nlHeap->fwd = NULL;
	nlHeap->bck = NULL;
	nlHeap->size = (size - sizeof(*nlHeap)) | ARENA_LARGE;
	nlHeap->flst = NULL;
	return (nlHeap);
}

t_heap	*new_heap(t_heap **restrict _pHeapHead, const size_t _alloc_size)
{

	if (_alloc_size <= _M_TINY_MAX_ALC_SIZE || _alloc_size <= _M_SMALL_MAX_ALC_SIZE) {
		t_heap	*newHeap = NULL;
	
		if (_alloc_size <= _M_TINY_MAX_ALC_SIZE) {
			newHeap = new_tiny_heap();
		}
		else if (_alloc_size <= _M_SMALL_MAX_ALC_SIZE) {
			newHeap = new_small_heap();
		}
		if (newHeap == NULL)
			return (NULL);
		t_heap	*prevHeap = NULL;
		while (*_pHeapHead) {
			prevHeap = *_pHeapHead;
			_pHeapHead = &prevHeap->fwd;
		}
		*_pHeapHead = newHeap;
		newHeap->bck = prevHeap;
		return (newHeap);
	}
	else if (_alloc_size > _M_SMALL_MAX_ALC_SIZE){
		return (new_large_heap(_alloc_size));
	}
	return (NULL);
}
