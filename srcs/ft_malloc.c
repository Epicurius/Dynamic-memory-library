/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 11:54:07 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/21 10:53:58 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

t_alloc	g_alloc =
{
	{NULL, NULL, NULL},
	PTHREAD_MUTEX_INITIALIZER
};

/*
 *	Searches through all the blocks in the zone and returns one if
 *	its free and if the memory size is same or less.
 */
static void	*find_space(t_zone *zone, size_t size)
{
	t_block	*block;

	while (zone)
	{
		block = (void *)zone + sizeof(t_zone);
		while (block)
		{
			if (block->free == TRUE && size <= block->size)
			{
				block->free = FALSE;
				block->size = size;
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
void	*alloc_amount(int type, size_t total, size_t size)
{
	void	*mem;
	t_zone	*zone;

	mem = find_space(g_alloc.zone[type], size);
	if (mem)
		return (mem);
	zone = create_new_zone(&g_alloc.zone[type], total);
	if (!zone)
		return (NULL);
	mem = find_space(g_alloc.zone[type], size);
	return (mem);
}

/*
 *	Mutes pthread to be safe.
 *	Check size type and send to alloc_amount() with corrent size.
 */
void	*malloc(size_t size)
{
	void	*mem;

	pthread_mutex_lock(&g_alloc.mutex);
	if (size <= 0)
		mem = NULL;
	else if (size <= MEM_TINY_MAX)
		mem = alloc_amount(MEM_TINY, MEM_TINY_ZONE, size);
	else if (size <= MEM_SMALL_MAX)
		mem = alloc_amount(MEM_SMALL, MEM_SMALL_ZONE, size);
	else
		mem = alloc_amount(MEM_LARGE, sizeof(t_block)
				+ sizeof(t_zone) + size, size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
