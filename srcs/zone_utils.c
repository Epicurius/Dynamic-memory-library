/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdmdev.h"

/*
 * Returns the total zone size. The zone size is the lowest possible amount of
 * 'PAGE_SIZE' that can fit the average block size times 'BLOCKS_PER_ZONE'.
 */
size_t get_zone_size(size_t min, size_t max)
{
	size_t block = sizeof(struct block) + (int)((min + max)  / 2);
	size_t zone = sizeof(struct zone) + block * BLOCKS_PER_ZONE;
	size_t page = PAGE_SIZE;
	size_t extra = (zone % page);
	if (extra == 0)
		return zone;
	return zone + page - extra;
}

/*
 * Returns the total zone size depending on the 'size' of a block.
 */
enum zone_type get_zone_type(size_t size)
{
	if (size <= MEM_TINY_MAX)
		return MEM_TINY;
	if (size <= MEM_SMALL_MAX)
		return MEM_SMALL;
	return MEM_LARGE;
}

/*
 * Allocate new zone with one free block.
 */
static struct zone *allocate_zone(size_t size)
{
	struct zone *zone;
	struct block *block;

	zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
				-1, 0);
	if (zone == MAP_FAILED)
		return NULL;
	zone->next = NULL;
	zone->end = (void *)zone + size;

	block = (void *)zone + sizeof(struct zone);
	block->next = NULL;
	block->free = TRUE;
	block->size = size - sizeof(struct zone) - sizeof(struct block);
	return zone;
}

/*
 * Return a new initialized zone.
 */
struct zone *new_zone(struct zone **head, size_t size)
{
	struct zone *zone;

	zone = allocate_zone(size);
	if (!head || !zone)
		return NULL;
	if (!*head)
		*head = zone;
	else {
		zone->next = *head;
		*head = zone;
	}
	return zone;
}
