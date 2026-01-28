#include "impl_mlc.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>

__attribute__((__always_inline__))
void	*_ft_align_memcpy(void *restrict _dst, void *restrict _src, const size_t _size)
{
	void		*dstSave = _dst;

# ifdef __AVX512F__
	const size_t	size512 = (_size / 64);
	for (size_t i = 0; i < size512; ++i) {
		__m512i data512 = _mm512_loadu_si512 (_src);
		_mm512_storeu_si512(_src, data512);
		_dst += 64;
		_src += 64;
	}
# endif
# ifdef __AVX2__
	if (_size & 32) {
		__m256i data256 = _mm256_loadu_si256(_src);
		_mm256_storeu_si256(_dst, data256);
		_dst += 32;
		_src += 32;
	}
# endif
# ifdef __AVX__
	if (_size & 16) {
		__m128i data128 = _mm_loadu_si128(_src);
		_mm_storeu_si128(_dst, data128);
		_dst += 16;
		_src += 16;
	}
# endif
	if (_size & 8) {
		*(uint64_t *)_dst = *(uint64_t *)_src;
		_dst += 8;
		_src += 8;
	}
	if (_size & 4) {
		*(uint32_t *)_dst = *(uint32_t *)_src;
		_dst += 4;
		_src += 4;
	}if (_size & 2) {
		*(uint64_t *)_dst = *(uint16_t *)_src;
		_dst += 2;
		_src += 2;
	}if (_size & 1) {
		*(uint8_t *)_dst = *(uint8_t *)_src;
		_dst += 1;
		_src += 1;
	}
	return (dstSave);
}

__attribute__((always_inline, const))
static int _get_arena(const size_t _size)
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
	void	*newPtr = malloc(_size);

	if (newPtr == NULL)
		return (_oldPtr);
	
	_ft_align_memcpy(_oldPtr, newPtr, sizeToCpy);
	free(_oldPtr);
	return (newPtr);
}

static void	_move_flst(t_flst *_old, const size_t _n)
{
	t_flst *new = _old->fwd;

	//ft_fprintf(2, "OldSize: %u\n", dd	(uint32_t)(_old->size & _M_SIZE_MASK));
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

__attribute__((always_inline, const))
static inline size_t	_round_size(size_t _size)
{
	if (!_size)
		_size =  (sizeof(t_flst) - sizeof(t_chunk));
	if (_size % _M_ALIGN)
		_size = (_size & ~_M_ALIGN_MASK) + _M_ALIGN;
	return (_size);
}

void	*_extand_chunk(t_chunk *_chunk, const size_t _size) {
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
	const int	newArena = _get_arena(_size);

	if (newArena != (chunk->size & _M_ARENA_MASK)) {
		return (_need_new_aloc(chunk, _ptr, _size));
	}


	if (_size <= chunk->size || _extand_chunk(chunk, _size) != NULL) {
		return (_ptr);
	}
	else {
		return (_need_new_aloc(chunk, _ptr, _size));
	}
}
