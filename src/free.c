#include "impl_mlc.h"

void	_insert_flst(t_flst *_pFree, t_heap *_heap)
{
	t_flst	*bck = _heap->flst;
	t_flst	*fwd = _heap->flst;

	while (fwd != NULL) {
		bck = fwd;
		fwd = fwd->fwd;
		if (_pFree < fwd)
			break;
	}
	_pFree->fwd = fwd;
	if (fwd != NULL)
		fwd->bck = _pFree;
	_pFree->bck = bck;
	if (bck != NULL) {
		bck->fwd = _pFree;
	} else {
		_heap->flst = _pFree;
	}
}


void	_free_chunk(t_chunk *_chunk, const size_t _arenaMask)
{
	pthread_mutex_lock(&arenas[ARENA_SMALL].mtx);
	
	//const void	*pBegin = (void *)_chunk;
	//const void	*pEnd = (void *)_chunk + (_chunk->size & ~_M_ALIGN_MASK) + sizeof(*_chunk);
	t_heap		*heap = _chunk->pheap;
	t_flst		*pFree = (void *)_chunk;

	pFree->fwd = NULL;
	pFree->bck = NULL;
	pFree->size &= ~_M_DATA_MASK;
	pFree->size |= _arenaMask | _M_FREE_MASK;

	debug_flst(heap);
	_insert_flst(pFree, heap); // Insert in flst

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
