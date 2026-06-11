#include "ft_printf.h"
#include "impl_mlc.h"
#include <time.h>

t_flst	*_find_in_flst(const size_t _size, const t_flst *restrict _flst, const t_flst *_bres, unsigned char *_placeFlst)
{
	while (_flst != NULL)
	{
		const size_t flstSize = _flst->size & _M_SIZE_MASK;

		if (flstSize == _size) {
			*_placeFlst = 1;
			return ((t_flst  *)_flst);
		}
		else if (flstSize > _size) {
			const unsigned int	hasPlaceFlst = (flstSize >= sizeof(t_flst) + _size);
	
			if (_bres == NULL || *_placeFlst == 0
				|| (flstSize < (_bres->size & _M_SIZE_MASK) && hasPlaceFlst)) {
				_bres = _flst;
				*_placeFlst = hasPlaceFlst;
			}
		}
		_flst = _flst->fwd;
	}
	return ((t_flst *)_bres);
}

t_flst	*_find_in_heaps(const size_t _size, t_heap *restrict _heap)
{
	unsigned char placeFlst = 0;
	t_flst	*bres = NULL;

	while (_heap != NULL) {
		bres = _find_in_flst(_size, _heap->flst, bres, &placeFlst);
		if (bres && (bres->size & _M_SIZE_MASK) == _size)
			break ;
		_heap = _heap->fwd;
	}
	return (bres);
}

static void	_update_flst(t_flst *_old, const size_t _size)
{
	t_flst *new = _old->fwd;

	if ((_old->size & _M_SIZE_MASK) >= sizeof(t_flst) + _size) {
		new = (void *)_old + sizeof(t_chunk) + _size;
		new->pheap = _old->pheap;
		new->bck = _old->bck;
		new->fwd = _old->fwd;
		new->size = (_old->size & _M_SIZE_MASK) - _size - sizeof(t_chunk);
		new->size |= _M_FREE_MASK | (_old->size & _M_ARENA_MASK);
	}
	if (_old->fwd)
		_old->fwd->bck = _old->bck;
	if (_old->bck != NULL) {
		_old->bck->fwd = new;
	}
	else {
		_old->pheap->flst = new;
	}
	if ((_old->size & _M_SIZE_MASK) < sizeof(t_flst) + _size) {
		_old->size &= _M_SIZE_MASK;
	}
	else {
		_old->size = _size;
	}
}

t_chunk	*_resrv_in_pheaps(const size_t _size, t_heap **restrict _pheap)
{
	unsigned char	placeFlst = 0;
	t_flst	*fptr = _find_in_heaps(_size, *_pheap);

	if (!fptr) {
		t_heap	*nheap = new_heap(_pheap, _size);
		 if (!nheap)
			 return (NULL);
		 fptr = _find_in_flst(_size, nheap->flst, NULL, &placeFlst);
	}
	_update_flst(fptr, _size);
	return ((t_chunk *)fptr);
}

void	*_mlc_tiny(const size_t _size)
{
	pthread_mutex_lock(&arenas[ARENA_TINY].mtx);

	t_chunk	*cres = _resrv_in_pheaps(_size, &(arenas[ARENA_TINY].heap));
	if (cres != NULL) {
			cres->size |= ARENA_TINY;
	}

	pthread_mutex_unlock(&arenas[ARENA_TINY].mtx);
	return ((void *)cres + sizeof(*cres));
}

void	*_mlc_small(const size_t _size)
{
	pthread_mutex_lock(&arenas[ARENA_SMALL].mtx);

	t_chunk	*cres = _resrv_in_pheaps(_size, &(arenas[ARENA_SMALL].heap));
	if (cres != NULL)
		cres->size |= ARENA_SMALL;

	pthread_mutex_unlock(&arenas[ARENA_SMALL].mtx);
	return ((void *)cres + sizeof(*cres));
}



void	*_mlc_large_mutex_locked(const size_t _size)
{
	t_large_heap	*newLHeap = new_large_heap(_size);

	if (newLHeap == NULL) {
		return (NULL);
	}
	if (!arenas[ARENA_LARGE].heap) {
		arenas[ARENA_LARGE].heap = (t_heap *)newLHeap;
	} else {
		t_large_heap	*heap = (t_large_heap *)arenas[ARENA_LARGE].heap;

		while (heap->fwd)
			heap = heap->fwd;
		heap->fwd = newLHeap;
		newLHeap->bck = heap;
	}
	return ((void*)newLHeap + sizeof(*newLHeap));
}

static inline void	*_mlc_large(const size_t _size) {
	void	*res;
	
	pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);
	res = _mlc_large_mutex_locked(_size);
	pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
	return (res);
}

void	*malloc(const size_t _size) {
	const size_t rounded_size = _round_size(_size);

	if (rounded_size <= _M_TINY_MAX_ALC_SIZE)
		return (_mlc_tiny(rounded_size));
	else if (rounded_size <= _M_SMALL_MAX_ALC_SIZE)
		return (_mlc_small(rounded_size));
	else
		return (_mlc_large(_size));
}
