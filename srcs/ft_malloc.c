/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
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

	if (size <= 0)
		return NULL;

	pthread_mutex_lock(&g_alloc.mutex);
	mem = _malloc(size);
	if (mem) {
		memset(mem, 0, size);

		if (hash)
			copy_malloc_hash((void *)mem - sizeof(t_block), hash);
	}
	pthread_mutex_unlock(&g_alloc.mutex);

	if (!mem)
		ERROR("'ft_malloc()' failed allocating memory");
	return mem;
}
