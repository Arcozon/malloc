#include "libft_malloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CALL_TEST_ONE_REALLOC(size, resize, src)	test_one_realloc(size, resize, src, sizeof(src))

void	test_one_realloc(const size_t size, const size_t resize, const void *src, const size_t sizeSrc) {
	const size_t	contentSize = sizeSrc > size ? size : sizeSrc;
	const size_t	sizeContentRealloc = contentSize > resize ? resize : contentSize;
	void	*alloc = malloc(size);

	printf("\n");
	printf("-- Testing Realloc  -- \n", size);
	printf("  - Malloc of %lu: %p\n", size, alloc);
	if (!alloc)	return ;
	memcpy(alloc, src, contentSize);
	show_alloc_mem();
	printf("  - Content: %.*s\n", (int)contentSize, alloc);
	alloc = realloc(alloc, resize);
	show_alloc_mem();
	printf("  - Realloc from %lu to %lu: %p\n", size, resize, alloc);
	if (!alloc)	return;
	printf("  - Content: %.*s\n", (int)sizeContentRealloc, alloc);
	free(alloc);
}


void	test_realloc(void) {
	const char longStr[] = "According to all known laws of aviation, there is no way a bee should be able to fly.\n\
Its wings are too small to get its fat little body off the ground.\n";


	dump_large_heap();
	CALL_TEST_ONE_REALLOC(16, 16, longStr);
	//CALL_TEST_ONE_REALLOC(16, 0, longStr);
	//CALL_TEST_ONE_REALLOC(16, 32, longStr);
	//CALL_TEST_ONE_REALLOC(16, 128, longStr);
}
