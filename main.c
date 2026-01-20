#include "libft_malloc.h"
#include "ft_printf.h"


int	main(void)
{
	char	*ptr = malloc(4);
	write(1, "a\n", 2);
	char	*ptr1 = malloc(4);
	write(1, "b\n", 2);
	char	*ptr2 = malloc(0);
	write(1, "c\n", 2);
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
