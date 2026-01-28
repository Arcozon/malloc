#include "ft_printf.h"
#include "impl_mlc.h"
#include "libft_malloc.h"
#include <string.h>

void test_dump_heap(void) {
  const size_t Nalc = 255;
  void *ptrS[Nalc];

  for (size_t i = 0; i < Nalc; ++i)
    ptrS[i] = malloc(16);
  for (size_t i = 0; i < Nalc; ++i)
    if ((i % 5) <= 1)
      free(ptrS[i]);
  dump_heap_ptr(ptrS[3]);
  //  show_alloc_mem();
}

// test if a malloc of 48 is alloced in block of 600 instead of a block of 64
void	test_defrag1(void) {
  void *p0 = malloc(64);
  void *p1 = malloc(48);
  void *p2 = malloc(48);

  dump_heap_ptr(p0);
  free(p0);
  dump_heap_ptr(p1);
  p0 = malloc(48);
  dump_heap_ptr(p0);
  void *p3 = malloc(64);
  dump_heap_ptr(p0);
}


int main(void)
{
	void *p0 = malloc(256);
	void *p1 = malloc(256);
	void *p2 = malloc(256);
	dump_large_heap();	
	free(p0);
	dump_large_heap();
	free(p1);
	dump_large_heap();
	free(p2);
	dump_large_heap();	
	test_defrag1();
}
