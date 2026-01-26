#include "ft_printf.h"
#include "impl_mlc.h"
#include "libft_malloc.h"
#include <string.h>

int main(void) {
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
