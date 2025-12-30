#include "libft_malloc.h"
#include <unistd.h>
int	main(void)
{
	char	*ptr = malloc(4);
	char	*ptr1 = malloc(4);
	char	*ptr2 = malloc(0);
	ptr[0] = '1';
	ptr[1] = '2';
	ptr[2] = '3';
	ptr[3] = 0;
	ft_fprintf(2, "Ptr:[%p], \"%s\"\n", ptr, ptr);
	ft_fprintf(2, "Ptr:[%p], \"%s\"\n", ptr1, ptr1);
	ft_fprintf(2, "Ptr:[%p], \"%s\"\n", ptr2, ptr2);
	ft_fprintf(2, "PtrDiff:[%u]\n", ptr1 - ptr);
	ft_fprintf(2, "PtrDiff:[%u]\n", ptr2 - ptr1);
	
	
}
