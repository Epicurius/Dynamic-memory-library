/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 * Returns the total zone size. The zone size is the lowest possible amount of
 * 'PAGE_SIZE' that can fit 'BLOCKS_PER_ZONE' amount of blocks.
 */
size_t get_zone_size(size_t size)
{
	size_t block = sizeof(t_block) + size;
	size_t zone = sizeof(t_zone) + block * BLOCKS_PER_ZONE;
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
 * Updated the next block. Next block might refer to the current block, if it
 * is split into two blocks.
 *
 * This function covers these scenarios:
 * 1. No next block, create one.
 * 2. Next block is free.
 *    2.1. Combine current block left overs into next block.
 *    2.2. No left overs use entire block.
 * 3. Current block is to large and can be split into two blocks.
 * 4. Fallback, nothing changes. Happens when current block is the exact size
 *    or there is no room to split the block.
 */
void resize_block(t_block *block, size_t size)
{
	t_block	*next = (void *)block + sizeof(t_block) + size;

	if (!block->next) {
		next->next = NULL;
		next->size = block->size - sizeof(t_block) - size;
		next->free = TRUE;

		block->size = size;
		block->next = next;
	}
	else if (block->next->free == TRUE) {
		if (next != block->next) {
			/*
			 * Move the next block 't_block' data into the leftover space in the
			 * current block. The two blocks might overlap, so we use temporary
			 * variables.
			 */
			size_t temp_size = block->next->size;
			t_block *temp_next = block->next->next;

			next->size = block->size - size + temp_size;
			next->next = temp_next;
			next->free = TRUE;
		}

		block->size = size;
		block->next = next;
	}
	else if (((void *)next + sizeof(t_block)) < (void *)block->next) {
		next->next = block->next;
		next->size = (void *)block->next - ((void *)next + sizeof(t_block));
		next->free = TRUE;

		block->size = size;
		block->next = next;
	}
	else /* Assume the block is the correct size. */
		block->size = size;
}

/*
 *	Allocate new zone with one free block.
 */
static void *new_zone(size_t size)
{
	t_zone		*zone;
	t_block		*block;

	zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
				-1, 0);
	if (zone == MAP_FAILED)
		return NULL;
	zone->next = NULL;
	zone->end = (void *)zone + size;

	block = (void *)zone + sizeof(t_zone);
	block->next = NULL;
	block->free = TRUE;
	block->size = size - sizeof(t_zone) - sizeof(t_block);
	return zone;
}

/*
 * Allocate zone and add it the to list 'head'.
 */
void *allocate_zone(t_zone **head, size_t size)
{
	t_zone *zone;

	zone = new_zone(size);
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
