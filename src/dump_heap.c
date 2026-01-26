#include "ft_printf.h"
#include "impl_mlc.h"
#include "show_mem.h"
#include <unistd.h>

# define LINE_LEN	64

static inline void	_populate_cLine(char cLine[LINE_LEN], const char _c) {
	const uint64_t	lc = _c;
	const uint64_t	c64 = lc << 56 | lc << 48 | lc << 40 | lc << 32 | lc << 24 | lc << 16 | lc << 8 | lc;

	size_t	i = 0;

	for ( ; i < LINE_LEN && (uintptr_t)(&cLine[i]) & ((sizeof(c64) / sizeof(_c)) - 1); ++i)
		cLine[i] = _c;
	for ( ; (LINE_LEN - i) < (sizeof(c64) / sizeof(_c)); i += (sizeof(c64) / sizeof(_c)))
		*(uint64_t *)(&cLine[i]) = c64;
	for ( ; i < LINE_LEN ; ++i)
		cLine[i] = _c;
}

void	_fprint_zone(const int _fd, const char *_color, const char _c, const size_t _nc, const size_t _iLine)
{
	static char	cLine[LINE_LEN + 1] = {0};

	const size_t	firstLen = (_iLine + _nc > LINE_LEN) ? (LINE_LEN - _iLine): _nc;
	const size_t	nLine = (_nc - firstLen) / LINE_LEN;
	const size_t	lastLen = (_nc - firstLen) % LINE_LEN;

	ft_fprintf(_fd, "%s", _color);

	for (size_t i = 0; i < firstLen; ++i)
		ft_fprintf(_fd, "%c", _c);
	if ((_iLine +_nc) >= LINE_LEN)
		ft_fprintf(_fd, "\n");
	if (nLine) {	
		if (cLine[0] != _c) {
			_populate_cLine(cLine, _c);
		}
		for (size_t i = 0; i < nLine; ++i) {
			ft_fprintf(_fd, "%s\n", cLine);
		}
	}
	for (size_t i = 0; i < lastLen; ++i)
		ft_fprintf(_fd, "%c", _c);
	ft_fprintf(_fd, RESET);

}

void	_fdump_heap_ptr(const int _fd, const t_heap *_heap, const t_chunk *_ptr) {
	const size_t	hChunkSize = (_heap->size + sizeof(*_heap)) / sizeof(t_chunk);
	const t_chunk	*iptr = (t_chunk *)_heap;
	size_t		i = 0;
	size_t		headerSize;
	size_t		dataSize;
	size_t		c;
	char		*clr;
	int		clrParity = 0;

	ft_fprintf(_fd , MAGENTA "%p <- " BOLD "%p" RBOLD " -> %p\n" RESET, _heap->bck, _heap, _heap->fwd);

	headerSize = sizeof(t_heap) / sizeof(*iptr);
	_fprint_zone(_fd, MAGENTA, 'H', headerSize, i);
	i += headerSize;

	while (i < hChunkSize) {
		dataSize = (iptr[i].size & _M_SIZE_MASK) / sizeof(t_chunk);
		if (iptr[i].size & _M_FREE_MASK) {
			c = 'F';
			clr = GREEN;
			headerSize = 2;
			--dataSize;
			clrParity = 0;
		}
		else {
			c = 'U';
			if (&iptr[i] == _ptr) {
				clr = BOLD YELLOW;
			}
			else if ((clrParity & 0b1) == 1){
				clr = CYAN;
			} else {
				clr = BLUE;
			}
			c = 'U';
			headerSize = 1;
			++clrParity;
		}
		ft_fprintf(_fd, BOLD);
		_fprint_zone(_fd, clr, c, headerSize, i % LINE_LEN);
		i += headerSize;
		_fprint_zone(_fd, clr, '0', dataSize, i % LINE_LEN);
		i += dataSize;
	}
	//ft_fprintf(_fd, "\n");
}

void	fdump_heap_ptr(const int _fd, const void *_ptr) {
	
	const t_chunk	*_pchunk = _ptr - sizeof(*_pchunk);
	pthread_mutex_lock(&arenas[_pchunk->size & _M_ARENA_MASK].mtx);

	_fdump_heap_ptr(_fd, _pchunk->pheap, _pchunk);

	pthread_mutex_unlock(&arenas[ARENA_TINY].mtx);
}

void	dump_heap_ptr(const void *_ptr) {
	fdump_heap_ptr(STDIN_FILENO, _ptr);
}
