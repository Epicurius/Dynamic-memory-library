/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 * Returns the reserved space.
 */
size_t get_alloc_size(void *ptr)
{
	return ((struct block *)(ptr - sizeof(struct block)))->size;
}

/*
 * Returns the max block size.
 */
size_t get_block_max(int type)
{
	if (type < MEM_TINY)
		return 0;
	if (type == MEM_TINY)
		return MEM_TINY_MAX;
	if (type == MEM_SMALL)
		return MEM_SMALL_MAX;
	/* type == MEM_LARGE */
	return -1;
}

/*
 * Splits block into 2 blocks and returns the first part.
 * Prerequisite:
 * - block->size - size >= sizeof(struct block) + MIN_SIZE
 */
struct block *split_block(struct block *block, size_t size)
{
	struct block *next = (void *)block + sizeof(struct block) + size;

	next->size = block->size - size - sizeof(struct block);
	next->next = block->next;
	next->free = TRUE;

	block->size = size;
	block->next = next;
	return block;
}

/*
 * Shrink block and add the extra to the next block.
 * Prerequisite:
 * - block->next
 * - block->next->free == TRUE
 * - size > MIN_SIZE && size <= MAX_SIZE
 */
struct block *shrink_block(struct block *block, size_t size)
{
	struct block tmp;
	tmp.size = block->next->size;
	tmp.next = block->next->next;

	struct block *next = (void *)block + sizeof(struct block) + size;
	next->size = tmp.size + block->size - size ;
	next->next = tmp.next;
	next->free = TRUE;

	block->size = size;
	block->next = next;
	return block;
}

/*
 * Down scale the block.
 */
struct block *downscale_block(struct block *block, size_t size, size_t min)
{
	if (size <= min)
		return NULL;

	if (block->next && block->next->free == TRUE) {
		/* Move next free block closer */
		return shrink_block(block, size);
	}

	if (block->size - size >= sizeof(struct block) + min) {
		/* Try spliting current block into 2 blocks */
		return split_block(block, size);
	}

	return NULL;
}

/*
 * Combine block with the next block.
 * Prerequisite:
 * - block->next
 * - block->next->free == TRUE
 */
struct block *combine_block(struct block *block, size_t size)
{
	block->size = size;
	block->next = block->next->next;
	return block;
}

/*
 * Grow block while shrinking the next block
 * Prerequisite:
 * - block->next
 * - block->next->free == TRUE
 * - block->size + block->next->size - size >= MIN_SIZE
 */
struct block *grow_block(struct block *block, size_t size)
{
	struct block *next = (void *)block + sizeof(struct block) + size;

	next->size = block->size + block->next->size - size;
	next->next = block->next->next;
	next->free = TRUE;

	block->size = size;
	block->next = next;
	return block;
}

/*
 * Up scale the block.
 */
struct block *upscale_block(struct block *block, size_t size, size_t min,
							size_t max)
{
	if (size > max)
		return NULL;

	if (block->next == NULL) {
		/* There is no more space left in zone */
		return NULL;
	}

	if (block->next->free == TRUE) {
		size_t comb_size = block->size + sizeof(struct block) + \
						   block->next->size;

		if (comb_size < size) {
			/* Both block combined don't have enough space */
			return NULL;
		}

		if (comb_size - size >= sizeof(struct block) + min) {
			/* Grow over the next block */
			return grow_block(block, size);
		}

		if (comb_size == size) {
			/* Combining the with the next block */
			return combine_block(block, size);
		}
	}
	return NULL;
}

/*
 * Try resizing the block, on success returns the block, else returns 'NULL'.
 */
struct block *resize_block(struct block *block, size_t size)
{
	if (block->size == size)
		return block;

	int type = get_zone_type(block->size);
	if (type == MEM_LARGE)
		return NULL;

	int min = get_block_max(type - 1);
	if (size > block->size) {
		return upscale_block(block, size, min, get_block_max(type));
	}
	return downscale_block(block, size, min);
}
