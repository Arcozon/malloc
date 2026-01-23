#include "ft_printf.h"
#include "impl_mlc.h"
#include "show_mem.h"


void	_print_free_chunk(const t_flst *_flst)
{
	const size_t	sizeFlst = _flst->size & _M_SIZE_MASK;
	const void	*beginFlst = (const void *)_flst + sizeof(t_chunk);
	const void	*endFlst = beginFlst + sizeFlst;

	ft_printf(GREEN "		");
//	ft_printf("Bck: %p | ", _flst->bck);
	ft_printf("[%p-%p], Size: %u", beginFlst, endFlst, (unsigned int)sizeFlst);
	//ft_printf("[%p-%p], Size: %u | ", beginFlst, endFlst, (unsigned int)sizeFlst);
//	ft_printf("Fwd: %p", _flst->fwd);
	ft_printf(RESET "\n");
}

void	_print_chunk(const t_chunk *_chunk)
{
	if (_chunk->size & _M_FREE_MASK) {
		 _print_free_chunk((const t_flst *)_chunk);	
	}
	else {
		const size_t	sizeChunk =  _chunk->size & _M_SIZE_MASK;
		const void	*beginChunk = (const void *)_chunk + sizeof(*_chunk);
		const void	*endChunk = beginChunk + sizeChunk;

		ft_printf(RED "		");
		ft_printf("[%p-%p], Size: %u", beginChunk, endChunk, (unsigned int)sizeChunk);
		ft_printf(RESET "\n");
	}
}

void	_show_heap(const t_heap *_heap, const unsigned int i)
{
	const size_t	sizeHeap = (_heap->size & _M_SIZE_MASK);
	const void	*endHeap = (void *)_heap + sizeHeap + sizeof(*_heap);

	ft_printf(MAGENTA "	Heap " BOLD "%u" RBOLD ": ", i);
	ft_printf("[%p-%p], Size: %u\n", _heap, endHeap, (unsigned int)(sizeHeap + sizeof(*_heap)));
	ft_printf(RESET);

	const t_chunk	*chunk = (void *)_heap + sizeof(*_heap);
	for (unsigned int j = 0; (const void *)chunk < endHeap; ++j) {
		_print_chunk(chunk);
		chunk = (void *)chunk + (chunk->size & _M_SIZE_MASK) + sizeof(*chunk);
		(void)j;
	}
}

void	_show_arena(t_arena *_arena)
{
	pthread_mutex_lock(&_arena->mtx);

	const t_heap	*heap = _arena->heap;

	if (heap == NULL) {
		ft_printf(RED BOLD "	No heap\n" RESET);
	}
	for (unsigned int i = 0; heap != NULL; ++i) {
		_show_heap(heap, i);
		heap = heap->fwd;
	}

	pthread_mutex_unlock(&_arena->mtx);
}

void	show_alloc_mem(void)
{
	ft_printf(BLUE "[" BOLD "TINY" RBOLD "]\n" RESET);
	_show_arena(&arenas[ARENA_TINY]);
//	ft_printf(BLUE "[" BOLD "SMALL" RBOLD "]\n" RESET);
//	_show_arena(&arenas[ARENA_SMALL]);
//	ft_printf(BLUE "[" BOLD "LARGE" RBOLD "]\n" RESET);
}
