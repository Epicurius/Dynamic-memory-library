/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

t_alloc	g_alloc =
{
	.zone = {NULL, NULL, NULL},
	.mutex = PTHREAD_MUTEX_INITIALIZER
};

/*
 *	Searches through all the blocks in the zone and returns one if
 *	its free and if the memory size is same or less.
 */
static void	*find_space(t_zone *zone, size_t size)
{
	t_block	*block;

	while (zone) {
		block = (void *)zone + sizeof(t_zone);
		while (block) {
			if (block->free == TRUE && size <= block->size) {
				block->free = FALSE;
				resize_block(block, size);
				return ((void *)block + sizeof(t_block));
			}
			block = block->next;
		}
		zone = zone->next;
	}
	return NULL;
}

/*
 * Returns a pointer to user memory after finding and reserving a block.
 */
static void *get_free_block(int type, size_t total, size_t size)
{
	void	*mem;
	t_zone	*zone;

	mem = find_space(g_alloc.zone[type], size);
	if (mem)
		return mem;
	zone = allocate_zone(&g_alloc.zone[type], total);
	if (!zone)
		return NULL;
	mem = find_space(g_alloc.zone[type], size);
	return mem;
}

/*
 * Private not thread safe 'malloc()'.
 */
void *_malloc(size_t size)
{
	if (size <= MEM_TINY_MAX)
		return get_free_block(MEM_TINY, MEM_TINY_ZONE, size);

	if (size <= MEM_SMALL_MAX)
		return get_free_block(MEM_SMALL, MEM_SMALL_ZONE, size);

	void *zone = allocate_zone(&g_alloc.zone[MEM_LARGE],
							   sizeof(t_zone) + sizeof(t_block) + size);
	if (!zone)
		return NULL;
	((t_block *)(zone + sizeof(t_zone)))->free = FALSE;
	return zone + sizeof(t_zone) + sizeof(t_block);
}

/*
 * Public thread safe 'malloc()'.
 */
void *malloc(size_t size)
{
	void *mem;

	if (size <= 0)
		return NULL;

	pthread_mutex_lock(&g_alloc.mutex);
	mem = _malloc(size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return mem;
}
