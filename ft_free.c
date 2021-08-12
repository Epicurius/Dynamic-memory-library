/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 10:52:52 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 14:29:44 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

/*
 *	Combine blocks if curr and/or prev are free.
*/
static void	merge(t_block *prev, t_block *curr)
{
	if (curr->next && curr->next->free == TRUE)
	{
		curr->memsize += sizeof(t_block) + curr->next->memsize;
		curr->next = curr->next->next;
		curr->checksum = (size_t)curr + curr->memsize;
	}
	if (prev && prev->free == TRUE)
	{
		prev->memsize += sizeof(t_block) + curr->memsize;
		prev->next = curr->next;
		prev->checksum = (size_t)prev + prev->memsize;
	}
}

static void	release_zone(t_zone **head, t_zone *zone, t_zone *prev)
{
	t_block		*block;
	char		*str;
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

static int	free_block_chain(t_block *block, void *ptr, size_t *size)
{
	t_block	*prev;

	prev = NULL;
	while (block)
	{
		if ((void *)block + sizeof(t_block) == ptr && !block->free)
		{
			block->free = TRUE;
			*size = block->memsize;
			merge(prev, block);
			return (1);
		}
		prev = block;
		block = block->next;
	}
	return (0);
}

static int	check_zone(t_zone **head, void *ptr, size_t *size)
{
	t_zone	*curr;
	t_zone	*prev;
	t_block	*block;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if ((void *)curr < ptr && ptr < curr->end)
		{
			block = (void *)curr + sizeof(t_zone);
			if (free_block_chain(block, ptr, size))
			{
				release_zone(head, curr, prev);
				return (1);
			}
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

/*
 *	Dont know where the memory is so checking from
 *	TINY -> SMALL -> LARGE -> ERROR
 */
int	ft_free(void *ptr)
{
	int		found;
	size_t	size;

	found = 0;
	pthread_mutex_lock(&g_alloc.mutex);
	if (ptr)
	{
		if (check_zone(&g_alloc.zone[TINY], ptr, &size))
			found = 1;
		else if (check_zone(&g_alloc.zone[SMALL], ptr, &size))
			found = 2;
		else if (check_zone(&g_alloc.zone[LARGE], ptr, &size))
			found = 3;
		else
			found = -1;
	}
	pthread_mutex_unlock(&g_alloc.mutex);
	return (found);
}
