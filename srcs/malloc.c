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
 * Searches through the zone 'zone' for a block that has minimum 'min' amount
 * of available space. Returns the block if found, else returns 'NULL'.
 */
static t_block *find_block(t_zone *zone, size_t size, size_t min)
{
	t_block	*block;

	block = (void *)zone + sizeof(t_zone);
	while (block) {
		if (block->free == TRUE) {
			if (size == block->size) {
				/* Block is the same size */
				block->free = FALSE;
				return block;
			}

			if (size < block->size && block->size - size >= min) {
				/* Block can be split into 2 blocks */
				split_block(block, size);
				block->free = FALSE;
				return block;
			}
		}
		block = block->next;
	}
	return NULL;
}

/*
 * Iterates through 'type' zones 'zone' searching for an available block.
 * Returns the block if found, else creates a new zone and block and returns it.
 */
static t_block *get_free_block(enum zone_type type, size_t min, size_t max,
							   size_t size)
{
	t_zone	*zone = g_alloc.zone[type];
	t_block *block;

	min += sizeof(t_block);
	while (zone) {
		if ((block = find_block(zone, size, min)))
			return block;
		zone = zone->next;
	}

	zone = new_zone(&g_alloc.zone[type], get_zone_size(max));
	if (!zone)
		return NULL;

	block = (void *)zone + sizeof(t_zone);
	split_block(block, size);
	block->free = FALSE;
	return block;
}

/*
 * Creates a zone with 1 reserved block. This function is 'MEM_LARGE' specific.
 */
static t_block *create_large_block(size_t size)
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
	return block;
}

/*
 * Private not thread safe 'malloc()'.
 */
void *_malloc(size_t size)
{
	t_block *block;

	if (size <= MEM_TINY_MAX)
		block = get_free_block(MEM_TINY, 0, MEM_TINY_MAX, size);
	else if (size <= MEM_SMALL_MAX)
		block = get_free_block(MEM_SMALL, MEM_TINY_MAX, MEM_SMALL_MAX, size);
	else
		block = create_large_block(size);

	if (!block)
		return NULL;
	return (void *)block + sizeof(t_block);
}

/*
 * Public thread safe 'malloc()'.
 */
void *malloc(size_t size)
{
	void *mem;

	if (!size)
		return NULL;

	pthread_mutex_lock(&g_alloc.mutex);
	mem = _malloc(size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return mem;
}
