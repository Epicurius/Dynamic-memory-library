/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 17/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libdm.h"

/*
 *	Loop through all the same type zones and there blocks until
 *	the block with the same hash is found.
 */
static void	*find_block(t_zone *zone, char *hash)
{
	t_block	*block;

	while (zone) {
		block = (void *)zone + sizeof(t_zone);
		while (block) {
			if (strcmp(block->str, hash) == 0)
				return (void *)block + sizeof(t_block);
			block = block->next;
		}
		zone = zone->next;
	}
	return NULL;
}

/*
 *	Find pointer to memory with hash.
 */
void	*ft_memfind(char *hash)
{
	void	*ptr;

	if (!hash)
		return NULL;

	ptr = NULL;
	pthread_mutex_lock(&g_alloc.mutex);
	for (int i = MEM_TINY; i <= MEM_LARGE; i++) {
		if ((ptr = find_block(g_alloc.zone[i], hash)))
			break ;
	}
	pthread_mutex_unlock(&g_alloc.mutex);
	return ptr;
}
