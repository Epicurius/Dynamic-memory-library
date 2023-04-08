/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 11:30:55 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/17 11:06:21 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Loop through all the same type zones and there blocks until where ptr
 *	points to is found.
 */
static int	find_at_zone(t_zone *zone, void *ptr,
	t_block **ptr_block, t_zone **ptr_zone)
{
	t_block	*block;

	while (zone)
	{
		if ((void *)zone < ptr && ptr < zone->end)
		{
			block = (void *)zone + sizeof(t_zone);
			while (block)
			{
				if ((void *)block + sizeof(t_block) == ptr)
				{
					*ptr_block = block;
					*ptr_zone = zone;
					return (1);
				}
				block = block->next;
			}
		}
		zone = zone->next;
	}
	return (0);
}

/*
 *	Find the block and zone where ptr points to.
 */
static int	find_block_and_zone(void *ptr, void *new,
	t_block **block, t_zone **zone)
{
	new = NULL;
	if (!ptr)
		return (0);
	if (find_at_zone(g_alloc.zone[MEM_TINY], ptr, block, zone))
		return (1);
	if (find_at_zone(g_alloc.zone[MEM_SMALL], ptr, block, zone))
		return (2);
	if (find_at_zone(g_alloc.zone[MEM_LARGE], ptr, block, zone))
		return (3);
	return (0);
}

/*
 *	If no pointer give malloc size and return it. (same as malloc(size)).
 *	Check with find_block_and_zone() to find the correct zone and block.
 *	If: size 0 free.
 *	else if: new size is less or same, free the overflow.
 *	else if: new size is more malloc copy memory and free old.
 */
void	*realloc(void *ptr, size_t size)
{
	void	*new;
	t_zone	*zone;
	t_block	*block;

	if (!ptr)
		return (malloc(size));
	if (find_block_and_zone(ptr, &new, &block, &zone))
	{
		if (!size)
			free(ptr);
		else if (size <= block->size)
		{
			block->size = size;
			update_next_block(zone, block);
			new = ptr;
		}
		else
		{
			new = malloc(size);
			if (new)
				ft_memcpy(new, ptr, ft_min(block->size, size));
			free(ptr);
		}
	}
	return (new);
}
