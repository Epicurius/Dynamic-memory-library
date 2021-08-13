/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 11:30:55 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 13:26:02 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Loop through all the same type zones and there blocks untill where ptr
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
int	find_block_and_zone(void *ptr, t_block **block, t_zone **zone)
{
	if (!ptr)
		return (0);
	if (find_at_zone(g_alloc.zone[TINY], ptr, block, zone))
		return (1);
	if (find_at_zone(g_alloc.zone[SMALL], ptr, block, zone))
		return (2);
	if (find_at_zone(g_alloc.zone[LARGE], ptr, block, zone)) 
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
void	*ft_realloc1(void *ptr, size_t size)
{
	void	*new;
	t_block	*block;
	t_zone	*zone;

	if (!ptr)
		return (ft_malloc(size));
	new = NULL;
	if (find_block_and_zone(ptr, &block, &zone))
	{
		if (size == 0)
			ft_free(ptr);
		else if (size <= block->memsize)
		{
			block->memsize = size;
			block->checksum = (size_t)block + block->memsize;
			update_next_block(zone, block);
			new = ptr;
		}
		else
		{
			new = ft_malloc(size);
			if (!new)
				return (NULL);
			ft_memcpy(new, ptr, block->memsize < size ? block->memsize : size);
			ft_free(ptr);
		}
	}
	return (new);
}