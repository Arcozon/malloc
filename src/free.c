#include "impl_mlc.h"

void	_insert_flst(t_flst *_pFree, t_heap *_heap)
{
	t_flst	*fwd = _heap->flst;
	t_flst	*bck = NULL;
	while (fwd != NULL && fwd < _pFree) {
		fwd = fwd->fwd;
	}
	
	_pFree->fwd = fwd;
	if (fwd != NULL) {
		fwd->bck = _pFree;
		bck = fwd->bck;
	}
	_pfree->bck = bck;
	if (bck != NULL) {
		bck->fwd = _pFree;
	} else {
		_heap->flst = _pFree;
	}
	ft_fprintf(2, "Bck: %p\nMe: %p\nFwd: %p\n\n", bck, _pFree, fwd);
}


void	_free_chunk(t_chunk *_chunk, const size_t _arenaMask)
{
	pthread_mutex_lock(&arenas[ARENA_SMALL].mtx);
	
	//const void	*pBegin = (void *)_chunk;
	//const void	*pEnd = (void *)_chunk + (_chunk->size & ~_M_ALIGN_MASK) + sizeof(*_chunk);
	t_heap		*heap = _chunk->pheap;
	t_flst		*pFree = (void *)_chunk;

	ft_fprintf(2, "Size: %u\n", (unsigned int)_chunk->size);
	debug_flst(heap);
	pFree->fwd = NULL;
	pFree->bck = NULL;
	pFree->size &= _M_SIZE_MASK;
	pFree->size |= _arenaMask | _M_FREE_MASK;

	_insert_flst(pFree, heap); // Insert in flst
	debug_flst(heap);

	pthread_mutex_unlock(&arenas[ARENA_SMALL].mtx);
}

void	_free_large(t_chunk *_chunk)
{
	(void)_chunk;
}

void	free(void *_ptr)
{
	if (_ptr)
	{
		t_chunk	*bchunk = _ptr - sizeof(*bchunk);

		if ((bchunk->size & _M_DATA_MASK) == ARENA_TINY) {
			_free_chunk(bchunk, ARENA_TINY);
		}
		else if ((bchunk->size & _M_DATA_MASK) == ARENA_SMALL) {
			_free_chunk(bchunk, ARENA_SMALL);
		}
		else {
			_free_large(bchunk);
		}
	}
}
