#include "impl_mlc.h"
void	debug_flst(const t_heap *heap)
{
	if (heap == NULL){
		ft_fprintf(2, "No heap for freelst\n");
		return ;
	}
	t_flst	*flst = heap->flst;
	int	cout = 0;
	while (flst != NULL)
	{
		ft_fprintf(2, "[%d]- bck:%14p | %14p[%5u] | fwd: %-14p\n", cout, flst->bck, flst, (unsigned int)(flst->size & ~_M_DATA_MASK) , flst->fwd);
		++cout;
		flst = flst->fwd;
	}
}

