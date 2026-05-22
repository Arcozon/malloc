#include "libft_malloc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void	test_realloc(void) {
	const char longStr[] = "According to all known laws of aviation, there is no way a bee should be able to fly.\n\
Its wings are too small to get its fat little body off the ground.\n";
	void	*tinyAlloc = malloc(16);
	void	*smallAlloc = malloc(33);
	void	*largeAlloc = malloc(170);
	
	memcpy(tinyAlloc, "Salut a tous\n", 14);
	memcpy(smallAlloc, "Salut a tous les amis\n", 23);
	memcpy(largeAlloc, longStr, sizeof(longStr));
	
	printf("TinyAlloc [%p]: %s", tinyAlloc, tinyAlloc);
	printf("SmallAlloc [%p]: %s", smallAlloc, smallAlloc);
	printf("largeAlloc [%p]: %s", largeAlloc, largeAlloc);
	
	void *tinyAlloc2 = realloc(tinyAlloc, 16);
	void *smallAlloc2 = realloc(smallAlloc, 33);
	void *largeAlloc2 = realloc(largeAlloc, 170);

	printf("TinyAlloc [%p]\n- Realloc [%p]\n", tinyAlloc, tinyAlloc2);
	printf("SmallAlloc [%p]\n-  Realloc [%p]\n", smallAlloc, smallAlloc2);
	printf("LargeAlloc [%p]\n-  Realloc [%p]\n", largeAlloc, largeAlloc2);
}
