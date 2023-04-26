/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

struct libdm g_libdm =
{
	.zone = {NULL, NULL, NULL},
	.mutex = PTHREAD_MUTEX_INITIALIZER
};

/*
 * Searches through the zone 'zone' for a block that has minimum 'min' amount
 * of available space. Returns the block if found, else returns 'NULL'.
 */
static struct block *find_block(struct zone *zone, size_t size, size_t min)
{
	struct block *block;

	block = (void *)zone + sizeof(struct zone);
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
static struct block *get_free_block(enum zone_type type, size_t min, size_t max,
									size_t size)
{
	struct zone	*zone = g_libdm.zone[type];
	struct block *block;

	min += sizeof(struct block);
	while (zone) {
		if ((block = find_block(zone, size, min)))
			return block;
		zone = zone->next;
	}

	zone = new_zone(&g_libdm.zone[type], get_zone_size(max));
	if (!zone)
		return NULL;

	block = (void *)zone + sizeof(struct zone);
	split_block(block, size);
	block->free = FALSE;
	return block;
}

/*
 * Creates a zone with 1 reserved block. This function is 'MEM_LARGE' specific.
 */
static struct block *create_large_block(size_t size)
{
	size_t zone_size;
	struct zone *zone;
	struct block *block;

	zone_size = sizeof(struct zone) + sizeof(struct block) + size;
	zone = new_zone(&g_libdm.zone[MEM_LARGE], zone_size);
	if (!zone)
		return NULL;

	block = (void *)zone + sizeof(struct zone);
	block->free = FALSE;
	return block;
}

/*
 * Private not thread safe 'malloc()'.
 */
void *_malloc(size_t size)
{
	struct block *block;

	if (size <= MEM_TINY_MAX)
		block = get_free_block(MEM_TINY, 0, MEM_TINY_MAX, size);
	else if (size <= MEM_SMALL_MAX)
		block = get_free_block(MEM_SMALL, MEM_TINY_MAX, MEM_SMALL_MAX, size);
	else
		block = create_large_block(size);

	return (void *)block + sizeof(struct block);
}

/*
 * Public thread safe 'malloc()'.
 */
void *malloc(size_t size)
{
	void *mem;

	if (!size)
		return NULL;

	pthread_mutex_lock(&g_libdm.mutex);
	mem = _malloc(size);
	pthread_mutex_unlock(&g_libdm.mutex);
	return mem;
}
