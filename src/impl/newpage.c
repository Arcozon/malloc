#include "impl_mlc.h"

__attribute__((always_inline, const))
static inline void	*_align_mem_16(void *_ptr)
{
	if (((uintptr_t)_ptr & _MASK_MLC_ALIGN) == 0)
		return (_ptr);
	else
		return ((void *)((uintptr_t)_ptr & ~_MASK_MLC_ALIGN) + _MLC_ALIGN);
}

t_zone	*_new_zone(const size_t _size)
{
	const long	pageSize = sysconf(_SC_PAGESIZE);
	size_t		nb_page;
	size_t		zone_size;
	t_zone		*zone;

	if (pageSize <= 0)
		return (NULL);

	nb_page = _size / (size_t)pageSize;
	if (nb_page * (size_t)pageSize != _size)
		++nb_page;
	zone_size = nb_page * (size_t)pageSize;
	zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0);
	ft_fprintf(2, "Nzone: %p,size %u, pageS: %d\n", zone, (unsigned int)zone_size, (int)pageSize);
	if (zone == MAP_FAILED)
		return (NULL);
	zone->next = NULL;
	zone->end = (void *)zone + zone_size;
	{
		t_header	*first_header = _align_mem_16((void *)zone + sizeof(*zone));

		first_header->size = zone_size - (sizeof(*zone) + sizeof(*first_header));
		first_header->isFree = 1;
	}
	return (zone);
}

t_zone	*_new_tiny_zone(void)
{
	return (_new_zone(_TINY_ZONE_SIZE));
}

t_zone	*_new_small_zone(void)
{
	return (_new_zone(_SMALL_ZONE_SIZE));
}

t_zone	*_new_ezone(const enum e_zone _eZone)
{
	if (_eZone == _MLC_ZONE_TINY)
		return (_new_zone(_TINY_ZONE_SIZE));
	else if (_eZone == _MLC_ZONE_SMALL)
		return (_new_zone(_SMALL_ZONE_SIZE));
	else
		return (NULL);
}
