#include "impl_mlc.h"

/*__attribute((always_inline, const))
static inline void	*_m_align_adrr(void *_adrr)
{
	if ((uintptr_t)_adrr & _M_ALIGN_MASK)
		_adrr += _M_ALIGN - ((uintptr_t)_adrr & _M_ALIGN_MASK);	
	return (_adrr);
}*/



t_flst	*_find_in_flst(const size_t _size, const t_flst *restrict _flst, const t_flst *_bres)
{
	while (_flst != NULL)
	{
		const size_t flstSize = _flst->size & _M_SIZE_MASK;
		if (flstSize == _size)
			return ((t_flst  *)_flst);
		else if (flstSize > _size && (_bres == NULL || flstSize < (_bres->size & _M_SIZE_MASK)))
			_bres = _flst;
		_flst = _flst->fwd;
	}
	return ((t_flst *)_bres);
}

t_flst	*_find_in_heaps(const size_t _size, t_heap *restrict _heap)
{
	t_flst	*bres = NULL;

	while (_heap != NULL) {
		bres = _find_in_flst(_size, _heap->flst, bres);
		if (bres && (bres->size & _M_SIZE_MASK) == _size)
			break ;
		_heap = _heap->fwd;
	}
	return (bres);
}

void	_del_flst(t_flst *_todel)
{
	if (_todel->fwd != NULL) {
		_todel->fwd->bck = _todel->bck;
	}
	if (_todel->bck != NULL) {
		_todel->bck = _todel->fwd;
	}
	else {
		_todel->pheap->flst = _todel->fwd;
	}
}

void	_update_flst(t_flst *_old, const size_t _size)
{
	t_flst *new = _old->fwd;

	ft_fprintf(2, "OldSize: %u\n", 	(uint32_t)(_old->size & _M_SIZE_MASK));
	if ((_old->size & _M_SIZE_MASK) >= sizeof(t_flst) + _size) {
		new = (void *)_old + sizeof(t_chunk) + _size;
//		ft_fprintf(2, "New chain\n");
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
}

t_chunk	*_resrv_in_pheaps(const size_t _size, t_heap **restrict _pheap)
{
	t_flst	*fptr = _find_in_heaps(_size, *_pheap);


	if (!fptr) {
		t_heap	*nheap = new_heap(_pheap, _size);
		 if (!nheap)
			 return (NULL);
		 fptr = _find_in_flst(_size, nheap->flst, NULL);
	}
//	ft_fprintf(2, "\nBefore Alc:\n");
//	debug_flst(*_pheap);
//	ft_fprintf(2, "Can flst\n");
	_update_flst(fptr, _size);
//	ft_printf("MeSize: %u, OldSize: %u\n", (unsigned int)_size, (unsigned int)(fptr->size & _M_SIZE_MASK) );
	if ((fptr->size & _M_SIZE_MASK) < sizeof(t_flst) + _size) {
		fptr->size &= _M_SIZE_MASK;
//		ft_printf("Size is NOT good for a new lst\n");
	}
	else {
		fptr->size = _size;
//		ft_printf("Size is good for a new lst\n");
	}
	//_del_flst(fptr);
//	ft_fprintf(2, "After Alc[%u]:\n", (unsigned int) _size);
//	debug_flst(*_pheap);
//	ft_fprintf(2, "\n");
	return ((t_chunk *)fptr);
}

void	*_mlc_tiny(const size_t _size)
{
	pthread_mutex_lock(&arenas[ARENA_TINY].mtx);

	t_chunk	*cres = _resrv_in_pheaps(_size, &(arenas[ARENA_TINY].heap));
	if (cres != NULL) {
			cres->size |= ARENA_TINY;
	}

	//ft_fprintf(2, "Ret: %p\n", ((void *)cres + sizeof(t_chunk)));
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
	//ft_fprintf(2, "Ret: %p\n", ((void *)cres + sizeof(t_chunk)));
	return ((void *)cres + sizeof(*cres));
}

void	*_mlc_large(const size_t _size)
{
	(void)_size;
	return (NULL);
}

__attribute__((always_inline, const))
static inline size_t	_round_size(size_t _size)
{
	if (!_size)
		_size =  (sizeof(t_flst) - sizeof(t_chunk));
	if (_size % _M_ALIGN)
		_size = (_size & ~_M_ALIGN_MASK) + _M_ALIGN;
	return (_size);
}
void	*malloc(size_t _size)
{
	//ft_fprintf(2, " -- TO ALLOC -- %u(%u)\n\n", (unsigned int)_round_size(_size), (unsigned int)_size);
	_size = _round_size(_size);
	if (_size <= _M_TINY_MAX_ALC_SIZE)
		return (_mlc_tiny(_size));
	else if (_size <= _M_SMALL_MAX_ALC_SIZE)
		return (_mlc_small(_size));
	else
		return (_mlc_large(_size));
}
