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
	const int cmpMem = memcmp(src, realc, sizeContentRealloc);
	printf("\tMemory copied to "BLUE"%lu"RESET": %s"RESET"", sizeContentRealloc, memOK[cmpMem != 0]);
	printf("\t["MAGENTA"%s"RESET"] -> ["MAGENTA"%s"RESET"]\n", arenaAlc, _getArena(realc));
	free(realc);
}

void	_test_realloc_null(const size_t size) {
	void	*alloc = realloc(NULL, size);

	printf("\n");
	printf("-- Testing Realloc from NULL to %u -> [%p] -- \n", (unsigned int)size, alloc);
	//show_alloc_mem();
	free(alloc);
}

void	_test_realloc(size_t bSize, size_t s0, size_t s1, size_t s2) {
	static const char longStr[] = "According to all known laws of aviation, there is no way a bee should be able to fly.\n\
Its wings are too small to get its fat little body off the ground.\n";

	printf(BOLD YELLOW "\n--- TESTING REALLOC WITH MALLOC(%lu) ---\n" RESET, bSize);
	CALL_TEST_ONE_REALLOC(bSize, s0, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s1, longStr);
	CALL_TEST_ONE_REALLOC(bSize, s2, longStr);
}
void	test_realloc(void) {
	_test_realloc(16, 16, 1025, 897);
	_test_realloc(1025, 16, 1025, 8197);
	_test_realloc(8197, 16, 1025, 8197);
	_test_realloc(8198, 8197, 8199, 16384);


	_test_realloc_null(0);
	_test_realloc_null(16);
	_test_realloc_null(33);
	_test_realloc_null(129);
}	
