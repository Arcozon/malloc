#include "impl_mlc.h"

# define GREEN "\e[33m"
# define RED "\e[32m"
# define RESET "\e[0m"

void	_show_arena(t_arena *_arena)
{
	pthread_mutex_lock(&_arena->mtx);

	const t_heap	*heap = _arena->heap;

	while (heap != NULL) {
		_show_heap(heap);
		heap = heap->fwd;
	}

	pthread_mutex_unlock(&_arena->mtx);
}

void	show_alloc_mem(void)
{
	
}
