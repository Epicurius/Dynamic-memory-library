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
	return ((t_block *)(ptr - sizeof(t_block)))->size;
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
 * - block->size - size >= sizeof(t_block) + MIN_SIZE
 */
t_block *split_block(t_block *block, size_t size)
{
	t_block *next = (void *)block + sizeof(t_block) + size;

	next->size = block->size - size - sizeof(t_block);
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
t_block *shrink_block(t_block *block, size_t size)
{
	t_block tmp;
	tmp.size = block->next->size;
	tmp.next = block->next->next;

	t_block *next = (void *)block + sizeof(t_block) + size;
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
t_block *downscale_block(t_block *block, size_t size, size_t min)
{
	if (size <= min)
		return NULL;

	if (block->next && block->next->free == TRUE) {
		/* Move next free block closer */
		return shrink_block(block, size);
	}

	if (block->size - size >= sizeof(t_block) + min) {
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
t_block *combine_block(t_block *block, size_t size)
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
t_block *grow_block(t_block *block, size_t size)
{
	t_block *next = (void *)block + sizeof(t_block) + size;

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
t_block *upscale_block(t_block *block, size_t size, size_t min, size_t max)
{
	if (size > max)
		return NULL;

	if (block->next == NULL) {
		/* There is no more space left in zone */
		return NULL;
	}

	if (block->next->free == TRUE) {
		size_t comb_size = block->size + sizeof(t_block) + block->next->size;

		if (comb_size < size) {
			/* Both block combined don't have enough space */
			return NULL;
		}

		if (comb_size - size >= sizeof(t_block) + min) {
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
