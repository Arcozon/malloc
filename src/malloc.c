
#include "impl_mlc.h"

__attribute__((always_inline, const))
static inline enum e_zone	_getZoneEnum(const size_t _size)
{
	if (_size <= _TINY_MAX_ALC_SIZE)
		return (_MLC_ZONE_TINY);
	else if (_size <= _SMALL_MAX_ALC_SIZE)
		return (_MLC_ZONE_SMALL);
	else
		return (_MLC_ZONE_LARGE);
}

__attribute__((always_inline, const))
static inline void	*_align_mem_16(void *_ptr)
{
	if (((uintptr_t)_ptr & _MASK_MLC_ALIGN) == 0)
		return (_ptr);
	else
		return ((void *)((uintptr_t)_ptr & ~_MASK_MLC_ALIGN) + _MLC_ALIGN);
}

void	*_reserve_mem(t_header *_to_res, const size_t _size, const void *_zone_end)
{
	(void)_zone_end;
	{
		const t_header	*next_header = _align_mem_16((void *)_to_res + sizeof(*_to_res) + _to_res->size);
		t_header	*new_header;
		
		ft_fprintf(2, "HeadShift: [%u]\n", (unsigned int)(sizeof(*_to_res) + _size));
		ft_fprintf(2, "Size: [%u]\n", (unsigned int)(_size));
		ft_fprintf(2, "OldSize: [%u]\n", (unsigned int)(_to_res->size));

		ft_fprintf(2, "pToRes: [%p]\n", _to_res);
		new_header = _align_mem_16((void *)_to_res + sizeof(*_to_res) + _size);
		ft_fprintf(2, "NwHead: [%p]\n", new_header);
		
		ft_fprintf(2, "P[%p]\nA[%p]\n", next_header, new_header);
		if ((void *)new_header + _MLC_ALIGN < (void *)next_header)
		{
			new_header->isFree = 1;
			new_header->size = _to_res->size - ((void *)new_header - (void *)_to_res) - sizeof(*new_header);
			ft_fprintf(2, "oldSize: %u, ", (unsigned int)_to_res->size);
			ft_fprintf(2, "NSize: %u\n", (unsigned int)new_header->size);
		}
		ft_fprintf(2, "Done\n\n");
	}
	_to_res->size = _size;
	_to_res->isFree = 0;
	return ((void *)_to_res + sizeof(*_to_res));
}

void	*_find_space(const size_t _size, t_zone **_pZone, const enum e_zone _eZone)
{
	void		*zone_end;
	t_header	*header;
	t_header	*best_match = NULL;
	void		*bm_zone_end;
	int		done = 0;

	while (*_pZone != NULL)
	{
		zone_end = (*_pZone)->end;
		header = _align_mem_16((void *)*_pZone + sizeof(**_pZone));

		ft_fprintf(2, "EndZone: [%p]\n", zone_end);
		while ((void *)header < zone_end)
		{
		ft_fprintf(2, "Bfrasdd: [%p] - %u\n", header, (unsigned int)header->size);
			if (header->isFree && sizeof(*header) >= _size)
			{
				if (best_match == NULL || best_match->size > header->size)
				{
					best_match = header;
					bm_zone_end = zone_end;
				}
				if (best_match->size == _size)
				{
					done = 1;
					break;
				}
			}
//			ft_fprintf(2, "Bfr:   %p[%u]\n", header, (unsigned int)(uintptr_t)header);
//			ft_fprintf(2, "Shift: [%u]\n", (unsigned int)(sizeof(*header) + header->size));
			header = _align_mem_16((void *)header + sizeof(*header) + header->size);
//			ft_fprintf(2, "Aft:   %p[%u]\n\n", header, (unsigned int)(uintptr_t)header);
		}
		if (done)
			break;
		_pZone = &(*_pZone)->next;
	}
	if (best_match == NULL)
	{
		t_zone *new_zone = _new_ezone(_eZone);
		if (!new_zone)
			return (NULL);
		*_pZone = new_zone;
		best_match = _align_mem_16((void*)new_zone + sizeof(*new_zone));
		bm_zone_end = new_zone->end;
	}
	write(2, "\n", 1);
	return (_reserve_mem(best_match, _size, bm_zone_end));
}

void	*_mlc_tiny(const size_t _size)
{
	static t_zone	*_tiny = NULL;

	return (_find_space(_size, &_tiny, _MLC_ZONE_TINY));
}

void	*_mlc_small(const size_t _size)
{
	(void)_size;
	return (NULL);
}

void	*_mlc_large(const size_t _size)
{
	(void)_size;
	return (NULL);
}

void	*malloc(size_t _size)
{
	if (_size <= _TINY_MAX_ALC_SIZE)
		return (_mlc_tiny(_size));
	else if (_size <= _SMALL_MAX_ALC_SIZE)
		return (_mlc_small(_size));
	else
		return (_mlc_large(_size));
	(void)_getZoneEnum(_size);
}
