/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:53:00 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 13:52:28 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

void	print_memory_alloc(size_t size, t_zone *zone)
{
	printf("  mmap : %p (%lu)\n", zone, size);
}

void	*new_zone(size_t size)
{
	t_zone		*new;
	t_block		*block;

	new = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new == MAP_FAILED)
		return (NULL);
	print_memory_alloc(size, new);
	new->next = NULL;
	new->end = (void *)new + size;
	block = (void *)new + sizeof(t_zone);
	block->memsize = size - sizeof(t_zone) - sizeof(t_block);
	block->free = TRUE;
	block->next = NULL;
	block->checksum = (size_t)block + block->memsize;
	return (new);
}

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