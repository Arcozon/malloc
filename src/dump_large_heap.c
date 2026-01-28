#include "impl_mlc.h"
#include "show_mem.h"
#include <time.h>

void fdump_large_heap(const int _fd) {
  pthread_mutex_lock(&arenas[ARENA_LARGE].mtx);

  const t_heap *heap = arenas[ARENA_LARGE].heap;

  ft_fprintf(_fd, BLUE "[" BOLD "LARGE" RBOLD "]\n" RESET);
  if (heap == NULL) {
    ft_fprintf(_fd, BOLD RED "	NONE\n" RESET);
  } else {
    for (unsigned int i = 0; heap != NULL; ++i) {
      const size_t sizeHeap = (heap->size & _M_SIZE_MASK);
      const void *endHeap = (void *)heap + sizeHeap + sizeof(*heap);

      ft_printf(MAGENTA "	Heap " BOLD "%u" RBOLD ": ", i);
      ft_printf("[%p-%p], Size: %u\n", heap, endHeap,
                (unsigned int)(sizeHeap + sizeof(*heap)));
      ft_printf(RESET);
      heap = heap->fwd;
    }
  }

  pthread_mutex_unlock(&arenas[ARENA_LARGE].mtx);
}

void dump_large_heap(void) { fdump_large_heap(STDOUT_FILENO); }
