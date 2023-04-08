/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 16/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libdm.h"

void	copy_malloc_hash(t_block *block, char *hash)
{
	int	i;
	int	len;

	i = -1;
	len = strlen(hash);
	len > 4 ? len = 4 : 0;
	while (++i < len)
		block->str[i] = hash[i];
}

void	*ft_malloc(size_t size, char *hash)
{
	void	*mem;

	pthread_mutex_lock(&g_alloc.mutex);
	if (size <= 0)
		mem = NULL;
	else if (size <= MEM_TINY_MAX)
		mem = alloc_amount(MEM_TINY, MEM_TINY_ZONE, size);
	else if (size <= MEM_SMALL_MAX)
		mem = alloc_amount(MEM_SMALL, MEM_SMALL_ZONE, size);
	else
		mem = alloc_amount(MEM_LARGE, sizeof(t_block)
				+ sizeof(t_zone) + size, size);
	if (!mem)
		exit(1);

	if (hash)
		copy_malloc_hash((void *)mem - sizeof(t_block), hash);
	memset(mem, 0, size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return mem;
}
