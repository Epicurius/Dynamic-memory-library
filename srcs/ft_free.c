/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 10:52:52 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 13:35:29 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Combine blocks if an adjacent block is free.
 */
static void	merge_free_adjacent_blocks(t_block *prev, t_block *curr)
{
	if (curr->next && curr->next->free == TRUE)
	{
		curr->size += sizeof(t_block) + curr->next->size;
		curr->next = curr->next->next;
		//curr->checksum = (size_t)curr + curr->size;
	}
	if (prev && prev->free == TRUE)
	{
		prev->size += sizeof(t_block) + curr->size;
		prev->next = curr->next;
		//prev->checksum = (size_t)prev + prev->size;
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
	while (block)
	{
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
	while (block)
	{
		if ((void *)block + sizeof(t_block) == ptr && !block->free)
		{
			block->free = TRUE;
			merge_free_adjacent_blocks(prev, block);
			return (1);
		}
		prev = block;
		block = block->next;
	}
	return (0);
}

/*
 *	Loop through all the zones and chek if ptr is inside.
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
	while (zone)
	{
		if ((void *)zone < ptr && ptr < zone->end)
		{
			block = (void *)zone + sizeof(t_zone);
			if (free_block_chain(block, ptr))
			{
				release_zone(head, zone, prev);
				return (1);
			}
		}
		prev = zone;
		zone = zone->next;
	}
	return (0);
}

/*
 *	Dont know where the memory is, so checking starting from:
 *	TINY -> SMALL -> LARGE -> ERROR
 */
void	free(void *ptr)
{
	int	zone;

	zone = 0;
	pthread_mutex_lock(&g_alloc.mutex);
	if (ptr)
	{
		if (check_zone(&g_alloc.zone[TINY], ptr))
			zone = 1;
		else if (check_zone(&g_alloc.zone[SMALL], ptr))
			zone = 2;
		else if (check_zone(&g_alloc.zone[LARGE], ptr))
			zone = 3;
		else
			zone = -1;
	}
	pthread_mutex_unlock(&g_alloc.mutex);
}
