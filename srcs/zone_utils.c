/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:53:00 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/15 18:49:00 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Updated next block:
 *	If block is the last block in this zone.
 *	If block is NOT the last block in this zone.
 *	TODO: find the solution to hangin < 32 bytes
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
 *	Create a new zone and init it.
 */
void	*new_zone(size_t size)
{
	t_zone		*new;
	t_block		*block;

	new = mmap(0, size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new == MAP_FAILED)
		return (NULL);
	new->next = NULL;
	new->end = (void *)new + size;
	block = (void *)new + sizeof(t_zone);
	block->next = NULL;
	block->free = TRUE;
	block->memsize = size - sizeof(t_zone) - sizeof(t_block);
	block->checksum = (size_t)block + block->memsize;
	return (new);
}

/*
 *	Add the zone created buy new_zone() to zone list.
 */
void	*create_new_zone(t_zone **head, size_t size)
{
	t_zone	*new;

	new = new_zone(size);
	if (!head || !new)
		return (NULL);
	if (!*head)
		*head = new;
	else
	{
		new->next = *head;
		*head = new;
	}
	return (new);
}
