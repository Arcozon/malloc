#include "libft_malloc.h"
#include "ft_printf.h"
#include <string.h>

int	main(void)
{
	char	*pT = malloc(4);
	char	*pT1 = malloc(8);
	char	*pT2 = malloc(5);
//	char	*pS = malloc(33);
//	char	*pS1 = malloc(64);

//	strcpy(pT, "CAC");
//	strcpy(pT1, "Salut a toi");
//	strcpy(pT2, "C");
	
//	ft_fprintf(2, "pT:[%p], \"%s\"\n", pT, pT);
//	ft_fprintf(2, "pT1:[%p], \"%s\"\n", pT1, pT1);
//	ft_fprintf(2, "pT2:[%p], \"%s\"\n", pT2, pT2);
	ft_fprintf(2, "pT0:[%p]\n", pT);
	ft_fprintf(2, "pT1:[%p]\n", pT1);
	ft_fprintf(2, "pT2:[%p]\n\n", pT2);
	//ft_fprintf(2, "pT0-1 Diff:[%u]\n", pT1 - pT);
	//ft_fprintf(2, "pT1-2 Diff:[%u]\n\n", pT2 - pT1);

	ft_fprintf(2, "------- FREEEEEE 1-----------------------\n");
	//free(pT);
	free(pT1);
	ft_fprintf(2, "------- FREEEEEE 2-----------------------\n");
	//free(pT1);
	free(pT);
	//pT = malloc(32);
	ft_fprintf(2, "------- FREEEEEE 3-----------------------\n");
	free(pT2);
	//ft_fprintf(2, "------- FREEEEEE 4-----------------------\n");
	//free(pT);

}
