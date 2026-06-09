#include "impl_mlc.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>

__attribute__((__always_inline__))
static inline void	*_ft_align_memcpy(void *restrict _dst, void *restrict _src, size_t _size) {
	void		*dstSave = _dst;

# ifdef __AVX512F__
	for (; _size >= 64; size -= 64) {
		__m512i data512 = _mm512_loadu_si512 (_src);
		ft_fprintf(2, "aa \n");
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
static inline void	*_need_new_aloc(t_chunk *_oldChunk, void *_oldPtr, size_t _size)
{
	const size_t	oldSize = (_oldChunk->size & _M_SIZE_MASK);
	const size_t	sizeToCpy = (_size < oldSize) ? _size : oldSize;
	void		*newPtr = malloc(_size);

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
static inline void	*_extandChunk(t_chunk *_chunk, const size_t _size) {
	const int idArena = _chunk->size & _M_ARENA_MASK;

	if (idArena == ARENA_LARGE) {
		return (_extandLarge(_chunk, _size));
	}
	pthread_mutex_lock(&arenas[idArena].mtx);

	t_chunk	*fwd = (void *)_chunk + (_chunk->size & _M_SIZE_MASK);
	t_heap	*heap = _chunk->pheap;
	void	*endHeap = (void *)heap + heap->size + sizeof(*heap);

	if ((void *)fwd >= endHeap || !(fwd->size & _M_FREE_MASK)
		|| _size > ((fwd->size & _M_SIZE_MASK) + (_chunk->size &_M_SIZE_MASK))) {
		pthread_mutex_unlock(&arenas[idArena].mtx);
		return (NULL);
	}

	pthread_mutex_unlock(&arenas[idArena].mtx);
	return ((void *)_chunk + sizeof(*_chunk));
}

void	*_realloc_large(void *_oldPtr, const size_t _size) {
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
static inline void	*_resizeChunk(t_chunk *chunk, const size_t _size, const int oldArena) {
	chunk	// Lock -> unlock
	if (size <= alreadyAlocked) {
		shrink or od nothing
		and return
	} else
		try to extand
	else
	return (NULL);
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
	const int	newArena = _getArena(_size);

	if (newArena != oldArena) {
		return (_need_new_aloc(chunk, _ptr, _size));
	}
	void	*resizePtr = _resizeChunk(chunk, _size);
	if (resizePtr != NULL)
		return (resizePtr);
	return (_need_new_aloc(chunk, _ptr, _size));
}
