/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 11:54:07 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 13:19:22 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Searches through all the blocks in the zone and returns one if
 *	its free and if the memory size is same or less.
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
 *	If find_space() finds a block with same or less amount of space return it.
 *	else create a new block and return it.
 */
void	*alloc_amount(int type, size_t total, size_t memsize)
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

/*
 *	Mutes pthread to be safe.
 *	Check size type and send to alloc_amount() with corrent size.
 */
void	*ft_malloc(size_t size)
{
	void	*mem;
	
	pthread_mutex_lock(&g_alloc.mutex);
	if (size <= 0)
		mem = NULL;
	else if (size <= TINY_MAX)
		mem = alloc_amount(TINY, TINY_ZONE_SIZE, size);
	else if (size <= SMALL_MAX)
		mem = alloc_amount(SMALL, SMALL_ZONE_SIZE, size);
	else
		mem = alloc_amount(LARGE, BLOCK_SIZE + ZONE_SIZE + size, size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
