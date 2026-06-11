#include "ft_malloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ft_printf.h"
#include "impl_mlc.h"

#ifndef SHOW_MEM_H
# define SHOW_MEM_H

# define RESET	"\e[0m"

# define BOLD	"\e[1m"
# define RBOLD	"\e[22m"

# define RED		"\e[31m"
# define GREEN		"\e[32m"
# define YELLOW		"\e[33m"
# define BLUE		"\e[34m"
# define MAGENTA	"\e[35m"
# define CYAN		"\e[36m"

#endif

#define CALL_TEST_ONE_REALLOC(size, resize, src)	test_one_realloc(size, resize, src, sizeof(src))
const char *_getArena(void *ptr) {
	static const char	*arenaStr[3] = {"TINY", "SMALL", "LARGE"};
	
	t_chunk	*cPtr = ptr - sizeof(*cPtr);
	return (arenaStr[cPtr->size & _M_ARENA_MASK]);
}

void	test_one_realloc(const size_t size, const size_t resize, const void *src, const size_t sizeSrc) {
	static const char *memOK[2] = {GREEN"Success", RED"Error"};

	const size_t	contentSize = (sizeSrc > size) ? size : sizeSrc;
	const size_t	sizeContentRealloc = (contentSize > resize) ? resize : contentSize;
	void	*alc = malloc(size);

	if (!alc) {
		printf(BOLD RED"\t MALLOC FAIL\n" RESET);
		return ;
	}
	const char *arenaAlc =	_getArena(alc);
	memcpy(alc, src, contentSize);
	void *realc = realloc(alc, resize);
	printf("  - realloc("MAGENTA "%p"RESET", "BLUE "%lu" RESET ") = "MAGENTA"%p" RESET, alc, (unsigned int)resize, realc);
	if (!realc) {
		printf(BOLD RED"\t REALLOC FAIL\n" RESET);
		return ;
	}
	size_t	nSize = 0;
	if (resize > _M_SMALL_MAX_ALC_SIZE) {
		nSize = ((t_large_heap*)(realc - sizeof(t_large_heap)))->used;
	} else  {
		nSize = ((t_chunk*)(realc - sizeof(t_chunk)))->size & _M_SIZE_MASK;
	}
	printf("(%lu) ", nSize);
	const int cmpMem = memcmp(src, realc, sizeContentRealloc);
	printf("\tMemory copied to "BLUE"%lu"RESET": %s"RESET"", sizeContentRealloc, memOK[cmpMem != 0]);
	printf("\t["MAGENTA"%s"RESET"] -> ["MAGENTA"%s"RESET"]\n", arenaAlc, _getArena(realc));
	free(realc);
}

void	_test_realloc_null(const size_t size) {
	void	*alloc = realloc(NULL, size);

	printf("-- Testing Realloc from NULL to %u -> [%p] -- \n", (unsigned int)size, alloc);
	//show_alloc_mem();
	free(alloc);
}

void	_test_realloc(size_t bSize, size_t s0, size_t s1, size_t s2, size_t s3, size_t s4) {
	static const char longStr[] = "According to all known laws of aviation, there is no way a bee should be able to fly.\n\
Its wings are too small to get its fat little body off the ground.\n";

	printf(BOLD YELLOW "\n--- TESTING REALLOC WITH MALLOC(%lu) ---\n" RESET, bSize);
	CALL_TEST_ONE_REALLOC(bSize, s0, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s1, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s2, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s3, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s4, longStr);
}

void	_testReallocantExtand(const size_t size) {
	void	*ptr = malloc(size);
	void	*bloc = malloc(size);

	dump_heap_ptr(ptr);
	void	*ptrRealloc = realloc(ptr, size + 16);
	dump_heap_ptr(ptrRealloc);
	free(ptrRealloc);
	free(bloc);
}

void	_testNoFlst(const size_t size) {
	void	*p0 = malloc(size);
	void	*p1 = malloc(size);
	void	*p2 = malloc(size);

	free(p1);
	dump_heap_ptr(p2);
	void *relac0 = realloc(p0, size + 32);
	printf("New size %lu (tried to have %lu)\n", ((t_chunk *)(relac0 - sizeof(t_chunk)))->size &_M_SIZE_MASK, size + 32);
	dump_heap_ptr(p2);
	void *relac1 = realloc(p0, size + 16);
	printf("New size %lu (tried to have %lu)\n", ((t_chunk *)(relac1 - sizeof(t_chunk)))->size &_M_SIZE_MASK, size + 16);
	dump_heap_ptr(p2);
	void *relac2 = realloc(p0, size);
	printf("New size %lu (tried to have %lu)\n", ((t_chunk *)(relac2 - sizeof(t_chunk)))->size &_M_SIZE_MASK, size);
	dump_heap_ptr(p2);
	free(p2);
	free(relac2);
}

void	test_realloc(void) {
	// _testReallocantExtand(16);
	// _testReallocantExtand(1025);
	// _testNoFlst(16);

	_test_realloc(32, 16, 32, 48, 1025, 8197);
	_test_realloc(1042, 1042, 1025, 1058, 16, 8197);
	_test_realloc(8297, 16, 1025, 8197, 8399, 16999);
	printf("\n");
	_test_realloc_null(0);
	_test_realloc_null(16);
	_test_realloc_null(1025);
	_test_realloc_null(8197);
}	
