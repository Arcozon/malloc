#include "impl_mlc.h"
#include <stdint.h>

__attribute((always_inline, const))
static inline void	*_m_align_adrr(void *_adrr)
{
	if ((uintptr_t)_adrr & _M_ALIGN_MASK)
		_adrr += _M_ALIGN - ((uintptr_t)_adrr & _M_ALIGN_MASK);	
	return (_adrr);
}

t_fchunk	*_find_in_heap(const size_t _size, t_fchunk *flst, t_fchunk *bres)
{
	while (flst)
	{
		if (flst->size == _size)
			return (flst);
		else if ((!bres && flst->size > _size) || flst->size < bres->size)
			bres = flst;
		flst = flst->fwd;
	}
	return (bres);
}

t_fchunk	*_find_in_heaps(const size_t _size, t_heap *restrict _heap)
{
	t_fchunk	*bres = NULL;

	while (!_heap)
	{
		bres = _find_in_heap(_size, _heap->flst, bres);
		if (bres && bres->size == _size)
			break ;
		_heap = _heap->fwd;
	}
	return (bres);
}

void	*_resrv_in_pheaps(const size_t _size, t_heap **restrict _pheap)
{
	t_fchunk	*res = _find_in_heaps(_size, *_pheap);
	
	if (!res)
	{
		 t_heap *nheap = new_tiny_heap(arenas[ARENA_TINY].heap);
		 if (!nheap)
			 return (NULL);
		 nheap->fwd = *_pheap;
		 *_pheap = nheap;
		 res = _find_in_heap(_size, nheap->flst, NULL);
	}
	t_fchunk	nf = 
	if (res->size < _size + sizeof(t_fchunk))
	{
	}
	return ((void *)res + sizeof(t_chunk));
}

void	*_mlc_tiny(const size_t _size)
{
	pthread_mutex_lock(&arenas[ARENA_TINY].mtx);

	void	*res = _resrv_in_pheaps(_size, _(&arenas[ARENA_TINY].mtx);
	
	pthread_mutex_unlock(&arenas[ARENA_TINY].mtx);
	return (res);
}

void	*_mlc_small(const size_t _size)
{
	(void)_size;
	return (NULL);
}

void	*_mlc_large(const size_t _size)
{
	(void)_size;
	return (NULL);
}

void	*malloc(size_t _size)
{
	if (_size <= _M_TINY_MAX_ALC_SIZE)
		return (_mlc_tiny(_size));
	else if (_size <= _M_SMALL_MAX_ALC_SIZE)
		return (_mlc_small(_size));
	else

		return (_mlc_large(_size));
}
