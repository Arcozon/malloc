#include "impl_mlc.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>

__attribute__((__always_inline__))
static inline void	*_ft_align_memcpy(void *restrict _dst, void *restrict _src, size_t _size)
{
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
static inline int _get_arena(const size_t _size)
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

void	*_need_new_aloc(t_chunk *_oldChunk, void *_oldPtr, size_t _size)
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

void	_move_flst(t_flst *_old, const size_t _n)
{
	t_flst *new = _old->fwd;

	//ft_fprintf(2, "OldSize: %u\n", dd	(uint32_t)(_old->size & _M_SIZE_MASK));
	if ((_old->size & _M_SIZE_MASK) >= sizeof(t_flst) + _n) {
		new = (void *)_old + sizeof(t_chunk) + _n;
		new->pheap = _old->pheap;
		new->bck = _old->bck;
		new->fwd = _old->fwd; 
		new->size = (_old->size & _M_SIZE_MASK) - _n - sizeof(t_chunk);
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
	if ((_old->size & _M_SIZE_MASK) < sizeof(t_flst) + _n) {
		_old->size &= _M_SIZE_MASK;
	}
	else {
		_old->size = _n;
	}
}

__attribute__((always_inline))
static inline void	*_extand_chunk(t_chunk *_chunk, const size_t _size) {
	const int idArena = _chunk->size & _M_ARENA_MASK;

	if (idArena == ARENA_LARGE) {
		return (NULL);
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

void	*_realloc_large(t_chunk *_oldChunk, const size_t _size) {
	t_large_heap * const	oldHeap = ((void *)_oldChunk) - (sizeof(oldHeap) - sizeof(*_oldChunk));
	void	 		*oldPtr = (void *)_oldChunk + sizeof(*_oldChunk);


	pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);
	if ((oldHeap->size >= _size)) {
		oldHeap->used = _size;
		pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
		return ((void *)oldPtr);
	}
	void	*newAlloc = _mlc_large_mutex_locked(_size);
	pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
	if (newAlloc == NULL)
		return (NULL);
	_ft_align_memcpy(newAlloc, oldPtr, _size);
	free(oldPtr);
	return (newAlloc);
}

void	*realloc(void *_ptr, size_t _size)
{
	if (_ptr == NULL) {
		return (malloc(_size));
	}
	else if (_size == 0) {
		free(_ptr);
		return (NULL);
	}
	
	t_chunk		*chunk = _ptr - sizeof(*chunk);
	const int	oldArena = (chunk->size & _M_ARENA_MASK);
	const int	newArena = _get_arena(_size);

	if (oldArena == ARENA_LARGE) {
		return (_realloc_large(chunk, _size));
	}
	if (newArena != oldArena) {
		return (_need_new_aloc(chunk, _ptr, _size));
	}
	if (_size <= chunk->size || _extand_chunk(chunk, _size) != NULL) {
		return (_ptr);
	}
	else {
		return (_need_new_aloc(chunk, _ptr, _size));
	}
}
