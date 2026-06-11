#include "ft_printf.h"
#include "impl_mlc.h"
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "stdio.h"
#include <stdlib.h>

void	_cat_flst(t_flst *_pFree)
{
	const void	*pBegin = (void *)_pFree;
	const void	*pEnd = (void *)_pFree + (_pFree->size & _M_SIZE_MASK) + sizeof(t_chunk);
	t_flst	*bck = _pFree->bck;
	t_flst	*fwd = _pFree->fwd;

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
	if (_heap->flst != NULL) {
		const t_flst	*flst = _heap->flst;
		if ((void *)_heap + sizeof(*_heap) == (void *)flst) {
			const void	*endHeap = (void *)_heap + (_heap->size & _M_SIZE_MASK) + sizeof(*_heap);
			const void	*endFlst = (void *)flst + (flst->size & _M_SIZE_MASK) + sizeof(t_chunk);
			if (endHeap == endFlst) {
				t_heap	*fwdHeap = _heap->fwd;
				t_heap	*bckHeap = _heap->bck;
				
				if (fwdHeap == NULL && bckHeap == NULL) // If this is the only heap we have we dont free it (
					return ;			// 	Avoid to call mmap/munmap when you are alocating->freeing over and over again, keeping one free heap in case
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
					ft_fprintf(STDERR_FILENO, "ERROR: munmap failure\n");
			}
		}
	}
}

void	_free_chunk(t_chunk *_chunk, const size_t _arenaMask)
{
	pthread_mutex_lock(&arenas[_arenaMask].mtx);
	
	t_heap		*heap = _chunk->pheap;
	t_flst		*pFree = (void *)_chunk;

	pFree->fwd = NULL;
	pFree->bck = NULL;
	pFree->size &= _M_SIZE_MASK;
	pFree->size |= _arenaMask | _M_FREE_MASK;

	_insertFlst(pFree, heap);
	_cat_flst(pFree);
	_free_heap(heap, &arenas[_arenaMask].heap);
	pthread_mutex_unlock(&arenas[_arenaMask].mtx);
}

void	_free_large(void *toFree)
{
//	ft_printf("Free large\n");
	pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);
	
	t_large_heap	*heap = toFree - sizeof(*heap);	
	t_large_heap	*fwd = heap->fwd;	
	t_large_heap	*bck = heap->bck;	
	
	if (fwd != NULL) {
		fwd->bck = bck;
	}
	if (bck != NULL) {
		bck->fwd = fwd;
	}
	else {
		arenas[ARENA_LARGE].heap = (t_heap *)fwd;
	}
	
	pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);

	if (munmap(heap, (heap->size & _M_SIZE_MASK) + sizeof(*heap)) != 0)
		ft_fprintf(STDERR_FILENO, "ERROR: mmap failure %p\n", heap);
}

__attribute__((destructor(101)))
void	_unmapTinySmallHeap(void) {
	{
		pthread_mutex_lock(&arenas[ARENA_TINY].mtx);
		t_heap	*heap = arenas[ARENA_TINY].heap;

		if (heap != NULL) {
			const size_t heapSize = (heap->size & _M_SIZE_MASK) + sizeof(*heap);

			if (munmap(heap, heapSize))
				ft_fprintf(STDERR_FILENO, "ERROR: munmap failure\n");
	
		}
		pthread_mutex_unlock(&arenas[ARENA_TINY].mtx);
	}
	{
		pthread_mutex_lock(&arenas[ARENA_SMALL].mtx);
		t_heap	*heap = arenas[ARENA_SMALL].heap;

		if (heap != NULL) {
			const size_t heapSize = (heap->size & _M_SIZE_MASK) + sizeof(*heap);

			if (munmap(heap, heapSize))
				ft_fprintf(STDERR_FILENO, "ERROR: munmap failure\n");
	
		}
		pthread_mutex_unlock(&arenas[ARENA_SMALL].mtx);
	}
}

void	free(void *_ptr)
{
	if (_ptr)
	{
		t_chunk	*chunk = _ptr - sizeof(*chunk);
		// printf("Freeing %p [%lu] %d %d\n", _ptr, chunk->size & _M_DATA_MASK, ((uintptr_t)_ptr & _M_ALIGN_MASK) != 0, (chunk->size & _M_FREE_MASK) != 0);

		if (((uintptr_t)_ptr & _M_ALIGN_MASK) != 0 || (chunk->size & _M_FREE_MASK) != 0) {
			ft_fprintf(2, "Invalid free\n");
			abort();
		}
		
		if ((chunk->size & _M_ARENA_MASK) == ARENA_TINY) {
			_free_chunk(chunk, ARENA_TINY);
		}
		else if ((chunk->size & _M_ARENA_MASK) == ARENA_SMALL) {
			_free_chunk(chunk, ARENA_SMALL);
		}
		else {
			_free_large(_ptr);
		}
	}
}
