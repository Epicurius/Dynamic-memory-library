/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:53:00 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/26 16:22:25 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Updated next block:
 *	If block is the last block in this zone.
 *	If block is NOT the last block in this zone.
 *	TODO: find the solution to hangin < 24 bytes
 */
void	update_next_block(t_zone *zone, t_block *block)
{
	t_block	*new;

	new = (void *)block + sizeof(t_block) + block->size;
	if (!block->next && (void *)new + sizeof(t_block) < zone->end)
	{
		new->next = NULL;
		new->size = zone->end - ((void *)new + sizeof(t_block));
		new->free = TRUE;
		block->next = new;
	}
	else if (block->next && (void *)new + sizeof(t_block) < (void *)block->next)
	{
		new->next = block->next;
		new->size = (void *)block->next - ((void *)new + sizeof(t_block));
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

	new = mmap(NULL, size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0);
	if (new == MAP_FAILED)
		return (NULL);
	new->next = NULL;
	new->end = (void *)new + size;
	block = (void *)new + sizeof(t_zone);
	block->next = NULL;
	block->free = TRUE;
	block->size = size - sizeof(t_zone) - sizeof(t_block);
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
