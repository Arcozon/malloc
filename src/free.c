#include "ft_printf.h"
#include "impl_mlc.h"

void	_insert_flst(t_flst *_pFree, t_heap *_heap)
{
	t_flst	*fwd = _heap->flst;
	t_flst	*bck = NULL;

	while (fwd != NULL && fwd < _pFree) {
		bck = fwd;
		fwd = fwd->fwd;
	}
	
	if (fwd != NULL) {
		fwd->bck = _pFree;
	}
	_pFree->fwd = fwd;
	_pFree->bck = bck;
	if (bck != NULL) {
		bck->fwd = _pFree;
	} else {
		_heap->flst = _pFree;
	}
//	ft_fprintf(2, "\nBck: %p\n Me: %p\nFwd: %p\n\n", bck, _pFree, fwd);
}	

void	_cat_flst(t_flst *_pFree)
{
	const void	*pBegin = (void *)_pFree;
	const void	*pEnd = (void *)_pFree + (_pFree->size & _M_SIZE_MASK) + sizeof(t_chunk);
	t_flst	*bck = _pFree->bck;
	t_flst	*fwd = _pFree->fwd;

//	ft_fprintf(2, "Start: %p	End: %p\n", pBegin, pEnd);
	if (fwd == pEnd) {
		t_flst *fwd_fwd = fwd->fwd;

		_pFree->fwd = fwd_fwd;
		if (fwd_fwd != NULL) {
			fwd_fwd->bck = _pFree;
		}
		_pFree->size += (fwd->size & _M_SIZE_MASK) + sizeof(t_chunk);
		fwd = fwd_fwd;
	}
	if (bck != NULL) {
		const void	*bckEnd =  (void *)bck + (bck->size & _M_SIZE_MASK) + sizeof(t_chunk);

		if (bckEnd == pBegin) {	// Cat with bck
			bck->size += (_pFree->size & _M_SIZE_MASK) + sizeof(t_chunk);
			bck->fwd = fwd;
			if (fwd != NULL)
				fwd->bck = bck;
		}
	}
}


void	_free_heap(t_heap *_heap, t_heap **_pFheap)
{
//	ft_fprintf(2, "Is heap NULL\n");
	if (_heap->flst != NULL) {
		const t_flst	*flst = _heap->flst;
//		ft_fprintf(2, "Is flst at the start of heap\n");
		if ((void *)_heap + sizeof(*_heap) == (void *)flst) {
			const void	*endHeap = (void *)_heap + (_heap->size & _M_SIZE_MASK) + sizeof(*_heap);
			const void	*endFlst = (void *)flst + (flst->size & _M_SIZE_MASK) + sizeof(t_chunk);
//			debug_flst(_heap);
//			ft_fprintf(2, "Do they end at the same place\n");
//			ft_fprintf(2, "%p - %p\n", endHeap, endFlst);
			if (endHeap == endFlst) {
				t_heap	*fwdHeap = _heap->fwd;
				t_heap	*bckHeap = _heap->bck;
				
				if (fwdHeap != NULL) {
					fwdHeap->bck = bckHeap;
				}
				if (bckHeap != NULL) {
					bckHeap->fwd = fwdHeap;
				}
				else {
					*_pFheap = fwdHeap;
				}
				if (munmap(_heap, (_heap->size & _M_SIZE_MASK) + sizeof(*_heap)))
					ft_fprintf(2, "ERROR\n");
				ft_fprintf(2, "-- HEAP FREED --\n\n");
			}
		}
	}
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
	ft_fprintf(2, "\nAfter Insert:\n");
	debug_flst(heap);
	_cat_flst(pFree);
	ft_fprintf(2, "\nAfter Cat:\n");
	debug_flst(heap);
//	ft_fprintf(2, "\n");
	_free_heap(heap, &arenas[_arenaMask].heap);
	//debug_flst(arenas[_arenaMask].heap);
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
//		ft_fprintf(2,"Tofree: %p\n", bchunk);
		if ((bchunk->size & _M_DATA_MASK) == ARENA_TINY) {
			_free_chunk(bchunk, ARENA_TINY);
		}
		else if ((bchunk->size & _M_DATA_MASK) == ARENA_SMALL) {
			ft_printf("Je la");
			_free_chunk(bchunk, ARENA_SMALL);
		}
		else {
			_free_large(bchunk);
		}
	}
}
