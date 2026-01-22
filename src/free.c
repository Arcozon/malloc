#include "impl_mlc.h"

void	_insert_flst(t_flst *_pFree, t_heap *_heap)
{
	t_flst	*fwd = _heap->flst;
	t_flst	*bck = NULL;

	while (fwd != NULL && fwd < _pFree) {
		fwd = fwd->fwd;
	}
	
	ft_fprintf(2, "Bck: %p\nMe: %p\nFwd: %p\n\n", bck, _pFree, fwd);
	_pFree->fwd = fwd;
	if (fwd != NULL) {
		fwd->bck = _pFree;
		bck = fwd->bck;
	}
	_pFree->bck = bck;
	if (bck != NULL) {
		bck->fwd = _pFree;
	} else {
		_heap->flst = _pFree;
	}
}	

void	_cat_flst(t_flst *_pFree, t_heap *_heap)
{
	const void	*pBegin = (void *)_pFree;
	const void	*pEnd = (void *)_pFree + (_pFree->size & _M_SIZE_MASK) + sizeof(t_chunk);
	t_flst	*bck = _pFree->bck;
	t_flst	*fwd = _pFree->fwd;

	ft_fprintf(2, "Start: %p	End: %p\n", pBegin, pEnd);
	if (fwd == pEnd) {
		t_flst *fwd_fwd = fwd->fwd;

		_pFree->fwd = fwd_fwd;
		if (fwd_fwd) {
			fwd_fwd->bck = _pFree;
		}
		_pFree->size += (fwd->size & _M_SIZE_MASK) + sizeof(t_chunk);
	}
	if (bck != NULL) {
		const void	*bckEnd =  (void *)bck + (bck->size & _M_SIZE_MASK) + sizeof(*bck);

		if (bckEnd == pBegin) {	// Cat with bck
			;
		}
	}
	(void)_heap;

}
void	_free_chunk(t_chunk *_chunk, const size_t _arenaMask)
{
	pthread_mutex_lock(&arenas[_arenaMask].mtx);
	
	t_heap		*heap = _chunk->pheap;
	t_flst		*pFree = (void *)_chunk;

	ft_fprintf(2, "Size: %u\n", (unsigned int)_chunk->size);
	pFree->fwd = NULL;
	pFree->bck = NULL;
	pFree->size &= _M_SIZE_MASK;
	pFree->size |= _arenaMask | _M_FREE_MASK;

	ft_fprintf(2, "\nBefore:\n");
	debug_flst(heap);
	_insert_flst(pFree, heap);
	ft_fprintf(2, "\nInsert:\n");
	debug_flst(heap);
	_cat_flst(pFree, heap);
	ft_fprintf(2, "\nCat:\n");
	debug_flst(heap);
	ft_fprintf(2, "\n");

	pthread_mutex_unlock(&arenas[_arenaMask].mtx);
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
		ft_fprintf(2,"Tofree: %p\n", bchunk);
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
