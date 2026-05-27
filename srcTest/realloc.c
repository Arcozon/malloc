#include "libft_malloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ft_printf.h"

#define CALL_TEST_ONE_REALLOC(size, resize, src)	test_one_realloc(size, resize, src, sizeof(src))

void	test_one_realloc(const size_t size, const size_t resize, const void *src, const size_t sizeSrc) {
	const size_t	contentSize = (sizeSrc > size) ? size : sizeSrc;
	const size_t	sizeContentRealloc = (contentSize > resize) ? resize : contentSize;
	void	*alloc = malloc(size);

	printf("\n");
	printf("-- Testing Realloc (%u -> %u) -- \n", (unsigned int)size, (unsigned int)resize);
	printf("  - Malloc of %u: %p\n", (unsigned int)size, alloc);
	if (!alloc)	return ;
	memcpy(alloc, src, contentSize);
	show_alloc_mem();
	printf("  - Content:\"%.*s\"\n", (int)contentSize, (char *)alloc);
	alloc = realloc(alloc, resize);
	show_alloc_mem();
	printf("  - Realloc from %u to %u: %p\n", (unsigned int)size, (unsigned int)resize, alloc);
	if (!alloc)	return;
	printf("  - Content: \"%.*s\"\n", (int)sizeContentRealloc, (char *)alloc);
	free(alloc);
}

void	_test_realloc_null(const size_t size) {
	void	*alloc = realloc(NULL, size);

	printf("\n");
	printf("-- Testing Realloc from NULL to %u -> [%p] -- \n", (unsigned int)size, alloc);
	show_alloc_mem();
	free(alloc);
}

void	test_realloc(void) {
	const char longStr[] = "According to all known laws of aviation, there is no way a bee should be able to fly.\n\
Its wings are too small to get its fat little body off the ground.\n";

	CALL_TEST_ONE_REALLOC(16, 0, longStr);
	CALL_TEST_ONE_REALLOC(16, 16, longStr);
	CALL_TEST_ONE_REALLOC(16, 33, longStr);
	CALL_TEST_ONE_REALLOC(16, 129, longStr);

	CALL_TEST_ONE_REALLOC(32, 0, longStr);
	CALL_TEST_ONE_REALLOC(32, 16, longStr);
	CALL_TEST_ONE_REALLOC(32, 33, longStr);
	CALL_TEST_ONE_REALLOC(32, 129, longStr);

	CALL_TEST_ONE_REALLOC(129, 0, longStr);
	CALL_TEST_ONE_REALLOC(129, 16, longStr);
	CALL_TEST_ONE_REALLOC(129, 33, longStr);
	CALL_TEST_ONE_REALLOC(129, 129, longStr);
	CALL_TEST_ONE_REALLOC(4064, 4065, "Yo");

	_test_realloc_null(0);
	_test_realloc_null(16);
	_test_realloc_null(33);
	_test_realloc_null(129);
}	
