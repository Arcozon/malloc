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

int main(void) {
  void *p0 = malloc(64);
  void *p1 = malloc(48);
  void *p2 = malloc(48);

  dump_heap_ptr(p0);
  ft_printf("Je suis la !");
  free(p0);
  ft_printf("Je suis la !");
  dump_heap_ptr(p1);
  p0 = malloc(48);
  dump_heap_ptr(p0);
  void *p3 = malloc(64);
  dump_heap_ptr(p0);
}
