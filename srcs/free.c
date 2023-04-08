/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 08/12/2021 Niklas Neronin
 * Updated: 10/04/2023 Niklas Neronin
 */

#include "libdm.h"

/*
 *	Combine blocks if an adjacent block is free.
 */
static void	merge_free_adjacent_blocks(t_block *prev, t_block *curr)
{
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
 *	Reliase zone if not reserved blocks found.
 *	And relink if more than 1 zone.
 */
static void	release_zone(t_zone **head, t_zone *zone, t_zone *prev)
{
	t_block		*block;
	size_t		size;

	block = (void *)zone + sizeof(t_zone);
	while (block) {
		if (block->free == FALSE)
			return ;
		block = block->next;
	}
	if (prev)
		prev->next = zone->next;
	else
		*head = zone->next;
	size = (size_t)(zone->end - (void *)zone);
	munmap(zone, size);
}

/*
 *	Loop through all the blocks until you find the ptr.
 *	Set to free.
 *	Send to merge_free_adjacent_blocks() to merger block adjacent to ptr.
 */
static int	free_block_chain(t_block *block, void *ptr)
{
	t_block	*prev;

	prev = NULL;
	while (block) {
		if ((void *)block + sizeof(t_block) == ptr && !block->free) {
			block->free = TRUE;
			merge_free_adjacent_blocks(prev, block);
			return 1;
		}
		prev = block;
		block = block->next;
	}
	return 0;
}

/*
 *	Loop through all the zones and check if ptr is inside.
 *	If is send to free_block_chain().
 *	Check if there are any blocks left in zone.
 *	Else free zone.
 */
static int	check_zone(t_zone **head, void *ptr)
{
	t_zone	*zone;
	t_zone	*prev;
	t_block	*block;

	zone = *head;
	prev = NULL;
	while (zone) {
		if ((void *)zone < ptr && ptr < zone->end) {
			block = (void *)zone + sizeof(t_zone);
			if (free_block_chain(block, ptr)) {
				release_zone(head, zone, prev);
				return 1;
			}
		}
		prev = zone;
		zone = zone->next;
	}
	return 0;
}

/*
 * Private not thread safe 'free()'.
 */
void	_free(void *ptr)
{
	for (int i = MEM_TINY; i <= MEM_LARGE; i++) {
		if (check_zone(&g_alloc.zone[i], ptr))
			break ;
	}
}

/*
 *	Dont know where the memory is, so checking starting from:
 *	MEM_TINY -> MEM_SMALL -> MEM_LARGE -> ERROR
 */
void	free(void *ptr)
{
	if (!ptr)
		return ;

	pthread_mutex_lock(&g_alloc.mutex);
	_free(ptr);
	pthread_mutex_unlock(&g_alloc.mutex);
}
