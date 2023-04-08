
#include "libmem.h"

int	main(void)
{
	char *str[5];

	str[0] = malloc(10);
	str[1] = malloc(400);
	str[2] = realloc(str[2], 20);
	str[3] = ft_malloc(5, "STR");
	str[4] = malloc(2048);
	free(str[4]);
	str[4] = malloc(4096);
	show_alloc_mem_ex(MEM_SHOW_HASH | MEM_SHOW_FREE);
	ft_mempurge();
	return (1);
}
