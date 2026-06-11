#include "impl_mlc.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

__attribute__((__always_inline__))
static inline void	*_ft_align_memcpy(void *restrict _dst, void *restrict _src, size_t _size) {
	void		*dstSave = _dst;

# ifdef __AVX512F__
	for (; _size >= 64; size -= 64) {
		__m512i data512 = _mm512_loadu_si512 (_src);
		_mm512_storeu_si512(_drc, data512);
		_dst += 64;
		_src += 64;
	}
# endif
# ifdef __AVX2__
	for (; _size >= 32; _size -= 32) {
		__m256i data256 = _mm256_loadu_si256(_src);
		_mm256_storeu_si256(_dst, data256);
		_dst += 32;
		_src += 32;
	}
# endif
# ifdef __AVX__
	for (; _size >= 16; _size -= 16) {
		__m128i data128 = _mm_loadu_si128(_src);
		_mm_storeu_si128(_dst, data128);
		_dst += 16;
		_src += 16;
	}
# endif
	for (; _size >= 8; _size -= 8) {
		*(uint64_t *)_dst = *(uint64_t *)_src;
		_dst += 8;
		_src += 8;
	}
	if (_size & 4) {
		*(uint32_t *)_dst = *(uint32_t *)_src;
		_dst += 4;
		_src += 4;
	}if (_size & 2) {
		*(uint16_t *)_dst = *(uint16_t *)_src;
		_dst += 2;
		_src += 2;
	}if (_size & 1) {
		*(uint8_t *)_dst = *(uint8_t *)_src;
	}
	return (dstSave);
}

__attribute__((always_inline, const))
static inline int _getArena(const size_t _size)
{
	if (_size <= _M_TINY_MAX_ALC_SIZE){
		return (ARENA_TINY);
	}
	else if (_size <= _M_SMALL_MAX_ALC_SIZE) {
		return (ARENA_SMALL);
	}
	else
		return (ARENA_LARGE);	
}

__attribute__((always_inline))
static inline void	*_need_new_aloc(t_chunk *_oldChunk, void *_oldPtr, size_t _size) {
	const size_t	oldSize = (_oldChunk->size & _M_SIZE_MASK);
	const size_t	sizeToCpy = (_size < oldSize) ? _size : oldSize;
	void			*newPtr = malloc(_size);

	if (newPtr == NULL)
		return (NULL);
	_ft_align_memcpy(newPtr, _oldPtr, sizeToCpy);
	free(_oldPtr);
	return (newPtr);
}

__attribute__((always_inline))
static inline void	*_extandLarge(t_chunk *_chunk, const size_t _size) {
	void	*basePtr = (void *)_chunk + sizeof(*_chunk);
	void	*res = NULL;

	pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);
	t_large_heap	*heap = basePtr - sizeof(*heap);
	if (heap->size <= _size) {
		res = basePtr;
		heap->used = _size;
	}
	pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
	return (res);
}

__attribute__((always_inline))
static inline void	*_reallocLarge(void *_oldPtr, const size_t _size) {
	t_large_heap * const	oldHeap = _oldPtr - sizeof(*oldHeap);

	pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);
	if (((oldHeap->size & _M_SIZE_MASK) - sizeof(*oldHeap) >= _size)) {
		oldHeap->used = _size;
		pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
		return (_oldPtr);
	}
	void	*newAlloc = _mlc_large_mutex_locked(_size);
	const size_t	toCpy = (oldHeap->used > _size) ? _size : oldHeap->used;

	pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
	if (newAlloc == NULL)
		return (NULL);
	_ft_align_memcpy(newAlloc, _oldPtr, toCpy);
	free(_oldPtr);
	return (newAlloc);
}

__attribute__((always_inline))
static inline int	_canExtandChunk(const t_chunk *_chunk, const size_t _sizeExtand, const t_flst *fwd) {
	const t_heap	*heap = _chunk->pheap;
	const void		*endHeap = (void *)heap + heap->size + sizeof(*heap);

	if ((void *)fwd >= endHeap) { // If out of heap
		return (0);
	} else if ((fwd->size & _M_FREE_MASK) == 0) { // If is not free
		return (0);
	} //else if ((_sizeExtand + sizeof(t_flst)) >= ((fwd->size & _M_SIZE_MASK) + sizeof(*_chunk))) { // If not enought free space
		//return (0);
	//}
	(void)_sizeExtand;
	return (1);
}

__attribute__((always_inline))
static inline void	*_extandChunk(t_chunk *_chunk, const size_t _size) {
	const int	oldArena = _chunk->size & _M_ARENA_MASK;
	t_flst		*oFlst = (void *)_chunk + (_chunk->size & _M_SIZE_MASK) + sizeof(*_chunk);
	const size_t	sizeExtand = _size - (_chunk->size & _M_SIZE_MASK);
	
	if (!_canExtandChunk(_chunk, sizeExtand, oFlst))
		return (NULL);
	t_flst	*bck	= oFlst->bck;
	t_flst	*fwd	= oFlst->fwd;
	
	// printf("Oldflst: %p\nHepflst: %p\n", oFlst, _chunk->pheap->flst);
	if ((oFlst->size & _M_SIZE_MASK) + sizeof(t_chunk) <= sizeExtand + sizeof(t_flst)) { // No place for new Flst
		// printf("No place for new one \n"); //HERE
		if (bck != NULL) {
			bck->fwd = fwd;
		} else {
			_chunk->pheap->flst = fwd;
		}
		if (fwd != NULL)
			fwd->bck = bck;
		// printf("%p | %p\n", oFlst, (void *)oFlst + sizeof(*_chunk) + (oFlst->size & _M_SIZE_MASK));
		_chunk->size = ((void *)oFlst + (oFlst->size & _M_SIZE_MASK)) - (void *)_chunk;
		// printf("%lu\n", _chunk->size);
		_chunk->size |= oldArena;
	} else {	//Place for a new one
		// printf("Place for new one \n"); //HERE
		t_flst *nFlst = (void *)oFlst + sizeExtand;
		// printf("Nflst: %p\n", nFlst);
		// pthread_mutex_unlock(&arenas[ARENA_TINY].mtx);
		// dump_heap_ptr((void *)_chunk + sizeof(*_chunk));
		// pthread_mutex_lock(&arenas[ARENA_TINY].mtx);
		nFlst->pheap = oFlst->pheap; 
		nFlst->size = oFlst->size - sizeExtand;
		nFlst->bck = bck; 
		nFlst->fwd = fwd;
		if (bck != NULL) {
			// printf("BCK != NULL \n"); //HERE
			bck->fwd = nFlst;
		} else {
			_chunk->pheap->flst = nFlst;
			// printf("BCK == NULL \n"); //HERE
		}
		if (fwd != NULL)
			fwd->bck = nFlst;
		_chunk->size = _size | oldArena;
	}
	return ((void *)_chunk + sizeof(*_chunk));
}



__attribute__((always_inline))
static inline void	_joinFlst(t_chunk *chunk, t_flst *oldFlst, const size_t freedSize) {
	t_flst *newFlst = (void *)oldFlst - freedSize;
	t_flst *bck = oldFlst->bck;
	t_flst *fwd = oldFlst->fwd;
	
	// pthread_mutex_unlock(&arenas[chunk->size &_M_ARENA_MASK].mtx);
	// show_alloc_mem();
	// pthread_mutex_lock(&arenas[chunk->size &_M_ARENA_MASK].mtx);
	// ft_fprintf(2, "Freed %u\n", (unsigned int)freedSize);
	// ft_printf("%p | %p | %p | %u\n", oldFlst->bck, oldFlst->fwd, oldFlst->pheap, (unsigned int)(oldFlst->size));
	*newFlst = *oldFlst;
	newFlst->size += freedSize;
	// ft_printf("%p | %p | %p | %u\n", newFlst->bck, newFlst->fwd, newFlst->pheap, (unsigned int)(newFlst->size));
	if (bck != NULL) {
		bck->fwd = newFlst;
	} else {
		// ft_printf("pre\n");
		newFlst->pheap->flst = newFlst;
		// ft_printf("post\n");
	}
	if (fwd !=NULL) {
		fwd->bck = newFlst;
	}
	chunk->size -= freedSize;
	// pthread_mutex_unlock(&arenas[chunk->size &_M_ARENA_MASK].mtx);
	// show_alloc_mem();
	// pthread_mutex_lock(&arenas[chunk->size &_M_ARENA_MASK].mtx);
	// ft_fprintf(2, "Freed %u\n", (unsigned int)freedSize);
}

__attribute__((always_inline))
static inline void	*_shrinkChunk(t_chunk *chunk, const size_t _size, const size_t _oldSize) {
	const size_t	freedSize = _oldSize - _size;
	t_chunk			*nextChunk = (void *)chunk + sizeof(*chunk) + (_oldSize & _M_SIZE_MASK);

	if (nextChunk->size & _M_FREE_MASK) {
		// printf("extand Flst\n"); //HERE
		_joinFlst(chunk, (t_flst *)nextChunk, freedSize);
	} else if (freedSize >= sizeof(t_flst)) {
		// printf("New Flst\n"); //HERE
		t_flst *newFlst = (void *)chunk + sizeof(*chunk) + _size;

		// printf("C:%p F:%p\n", chunk, newFlst);
		newFlst->pheap = chunk->pheap;
		newFlst->size = (freedSize - sizeof(t_chunk)) | _M_FREE_MASK | (_oldSize &_M_ARENA_MASK);
		newFlst->bck = NULL;
		newFlst->fwd = NULL;
		_insertFlst(newFlst, chunk->pheap);
		chunk->size -= _oldSize - _size;
	}
	return ((void *)chunk + sizeof(*chunk));
}

__attribute__((always_inline))
static inline void	*_resizeChunk(t_chunk *chunk, const size_t _size, const size_t _oldSize, const int _arena) {
	if (_size <= _oldSize && _size + _M_ALIGN > _oldSize)
		return ((void*)chunk + sizeof(*chunk));

	void	*res = NULL;
	
	pthread_mutex_lock(&arenas[_arena].mtx);
	if (_size < _oldSize) {
		// printf("shrink\n"); //HERE
		res = _shrinkChunk(chunk, _size, _oldSize);
	} else {
		// printf("extand\n"); //HERE
		res = _extandChunk(chunk, _size);
	}
	pthread_mutex_unlock(&arenas[_arena].mtx);
	return (res);
}

void	*realloc(void *_ptr, size_t _size) {
	if (_ptr == NULL) {
		return (malloc(_size));
	} else if (_size == 0) {
		free(_ptr);
		return (NULL);
	}
	
	t_chunk		*chunk = _ptr - sizeof(*chunk);
	const int	oldArena = (chunk->size & _M_ARENA_MASK);
	const size_t	roundedSize = _round_size(_size);
	const int	newArena = _getArena(roundedSize);

	if (newArena != oldArena) {
		return (_need_new_aloc(chunk, _ptr, _size));
	} else if (oldArena == ARENA_LARGE) {
		return (_reallocLarge(_ptr, _size));
	}
	void	*resizePtr = _resizeChunk(chunk, roundedSize, chunk->size & _M_SIZE_MASK, oldArena);
	if (resizePtr != NULL)
		return (resizePtr);
	return (_need_new_aloc(chunk, _ptr, _size));
}
