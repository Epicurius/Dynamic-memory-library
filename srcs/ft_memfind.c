/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 *	Loop through all the same type zones and there blocks until
 *	the block with the same hash is found.
 */
static void *find_block(struct zone *zone, char *hash)
{
	struct block *block;

	while (zone) {
		block = (void *)zone + sizeof(struct zone);
		while (block) {
			if (strcmp(block->str, hash) == 0)
				return (void *)block + sizeof(struct block);
			block = block->next;
		}
		zone = zone->next;
	}
	return NULL;
}

/*
 *	Find pointer to memory with hash.
 */
void *ft_memfind(char *hash)
{
	void *ptr;

	if (!hash)
		return NULL;

	ptr = NULL;
	pthread_mutex_lock(&g_libdm.mutex);
	for (int i = MEM_TINY; i <= MEM_LARGE; i++) {
		if ((ptr = find_block(g_libdm.zone[i], hash)))
			break ;
	}
	pthread_mutex_unlock(&g_libdm.mutex);
	return ptr;
}
