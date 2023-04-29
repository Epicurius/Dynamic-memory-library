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
 * Searches through the zone 'zone' for a block that has minimum 'size' amount
 * of available space. Returns the pointer if found, else returns 'NULL'.
 */
static void	*find_block(t_zone *zone, size_t size)
{
	t_block	*block;

	block = (void *)zone + sizeof(t_zone);
	while (block) {
		if (block->free == TRUE && size <= block->size) {
			block->free = FALSE;
			resize_block(block, size);
			return ((void *)block + sizeof(t_block));
		}
		block = block->next;
	}
	return NULL;
}

/*
 * Returns a pointer to user memory after finding and reserving a block.
 */
static void *get_free_block(enum zone_type type, size_t max, size_t size)
{
	t_zone	*zone = g_alloc.zone[type];
	void	*mem;

	while (zone) {
		if ((mem = find_block(zone, size)))
			return mem;
		zone = zone->next;
	}

	zone = new_zone(&g_alloc.zone[type], get_zone_size(max));
	if (!zone)
		return NULL;

	return find_block(zone, size);
}

/*
 * Creates a zone with 1 reserved block. This function is 'MEM_LARGE' specific.
 */
static void *create_large_block(size_t size)
{
	t_zone  *zone;
	size_t  zone_size;
	t_block *block;

	zone_size = sizeof(t_zone) + sizeof(t_block) + size;
	zone = new_zone(&g_alloc.zone[MEM_LARGE], zone_size);
	if (!zone)
		return NULL;

	block = (void *)zone + sizeof(t_zone);
	block->free = FALSE;
	return ((void *)block + sizeof(t_block));
}

/*
 * Private not thread safe 'malloc()'.
 */
void *_malloc(size_t size)
{
	if (size <= MEM_TINY_MAX)
		return get_free_block(MEM_TINY, MEM_TINY_MAX, size);

	if (size <= MEM_SMALL_MAX)
		return get_free_block(MEM_SMALL, MEM_SMALL_MAX, size);

	return create_large_block(size);
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
