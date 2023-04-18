/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 * Set 'curr' block to free and merge any adjacent free blocks into one free
 * block.
 */
static void	free_block(t_block *curr, t_block *prev)
{
	curr->free = TRUE;

	if (curr->next && curr->next->free == TRUE) {
		curr->size += sizeof(t_block) + curr->next->size;
		curr->next = curr->next->next;
	}
	if (prev && prev->free == TRUE) {
		prev->size += sizeof(t_block) + curr->size;
		prev->next = curr->next;
	}
}

/*
 * Release allocated memory and relink the previous zone.
 */
static void free_zone(enum zone_type type, t_zone *zone, t_zone *prev)
{
	if (prev)
		prev->next = zone->next;
	else
		g_alloc.zone[type] = zone->next;

	if (munmap(zone, (size_t)(zone->end - (void *)zone)) == -1)
		ERROR("freeing zone type '%d' failed", type);
}

/*
 * This function checks the remaining blocks and return whether all block are
 * free.
 */
static int is_rest_free(t_block *block)
{
	while (block) {
		if (block->free == FALSE)
			return FALSE;
		block = block->next;
	}
	return TRUE;
}

/*
 * Private not thread safe 'free()'.
 */
void _free(void *ptr)
{
	t_block *block = ptr - sizeof(t_block);
	enum zone_type type = get_zone_type(block->size);

	if (block->free == TRUE)
		ERROR("pointer being freed was not allocated");

	t_zone *curr_zone = g_alloc.zone[type];
	t_zone *prev_zone = NULL;

	while (curr_zone) {
		if ((void *)block < (void *)curr_zone ||
			(void *)block > (void *)curr_zone->end) {
			prev_zone = curr_zone;
			curr_zone = curr_zone->next;
			continue ;
		}

		if (type == MEM_LARGE) {
			/* Large zones have only one block */
			free_zone(type, curr_zone, prev_zone);
			return ;
		}

		t_block *curr_block = (void *)curr_zone + sizeof(t_zone);
		t_block *prev_block = NULL;

		unsigned char zone_free = TRUE;
		while (curr_block) {
			if (curr_block != block) {
				if (curr_block->free == FALSE)
					zone_free = FALSE;
				prev_block = curr_block;
				curr_block = curr_block->next;
				continue ;
			}

			if (zone_free && prev_zone && is_rest_free(curr_block->next)) {
				/* There is more than one zone and the current zone is free */
				free_zone(type, curr_zone, prev_zone);
				return ;
			}

			/* Set current block to free */
			free_block(curr_block, prev_block);
			return ;
		}
		ERROR("pointer being freed was not allocated");
	}
	ERROR("pointer being freed was not allocated");
}

/*
 * Find and free memory pointed to by 'ptr'.
 */
void free(void *ptr)
{
	if (!ptr)
		return ;

	pthread_mutex_lock(&g_alloc.mutex);
	_free(ptr);
	pthread_mutex_unlock(&g_alloc.mutex);
}
