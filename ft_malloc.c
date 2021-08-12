/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 11:54:07 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 13:52:28 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Updated next block:
 *	If block is the last block in this zone.
 *	If block is NOT the last block in this zone.
 */
void	update_next_block(t_zone *zone, t_block *block)
{
	t_block	*new;

	new = (void *)block + sizeof(t_block) + block->memsize;
	if (!block->next && (void *)new + sizeof(t_block) < zone->end)
	{
		new->next = NULL;
		new->memsize = zone->end - ((void *)new + sizeof(t_block));
		new->checksum = (size_t)new + new->memsize;
		new->free = TRUE;
		block->next = new;
	}
	else if (block->next && (void *)new + sizeof(t_block) < (void *)block->next)
	{
		new->next = block->next;
		new->memsize = (void *)block->next - ((void *)new + sizeof(t_block));
		new->checksum = (size_t)new + new->memsize;
		new->free = TRUE;
		block->next = new;
	}
}

/*
 *	Searches throught the all the blocks in the zone and returns one if
 *	the its free and memsize fits.
 */
static void	*find_space(t_zone *zone, size_t memsize)
{
	t_block	*block;

	while (zone)
	{
		block = (void *)zone + sizeof(t_zone);
		while (block)
		{
			if (block->free == TRUE && memsize <= block->memsize)
			{
				block->free = FALSE;
				block->memsize = memsize;
				block->checksum = (size_t)block + memsize;
				update_next_block(zone, block);
				return ((void *)block + sizeof(t_block));
			}
			block = block->next;
		}
		zone = zone->next;
	}
	return (NULL);
}

/*
 *	If find space finds a blick with the correct amount of space return it.
 *	Else create a new block and use it.
 */
static void	*malloc_amount(int type, size_t total, size_t memsize)
{
	void	*mem;
	t_zone	*zone;

	mem = find_space(g_alloc.zone[type], memsize);
	if (mem)
		return (mem);
	zone = create_new_zone(&g_alloc.zone[type], total);
	if (!zone)
		return (NULL);
	mem = find_space(g_alloc.zone[type], memsize);
	return (mem);
}

void	*ft_malloc(size_t memsize)
{
	void	*mem;
	
	pthread_mutex_lock(&g_alloc.mutex);
	if (memsize <= 0)
		mem = NULL;
	else if (memsize <= TINY_MAX)
		mem = malloc_amount(TINY, TINY_ZONE_SIZE, memsize);
	else if (memsize <= SMALL_MAX)
		mem = malloc_amount(SMALL, SMALL_ZONE_SIZE, memsize);
	else
		mem = malloc_amount(LARGE, BLOCK_SIZE + ZONE_SIZE + memsize, memsize);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
