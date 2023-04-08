/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 17/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libmem.h"

/*
 *	Loop through all the same type zones and there blocks until
 *	the block with the same hash is found.
 */
static int	find_block(t_zone *zone, char *hash, void *ptr)
{
	t_block	*block;

	while (zone)
	{
		block = (void *)zone + sizeof(t_zone);
		while (block)
		{
			if (ft_strequ(block->str, hash))
			{
				ptr = (void *)block + sizeof(t_block);
				return (1);
			}
			block = block->next;
		}
		zone = zone->next;
	}
	return (0);
}

/*
 *	Find pointer to memory with hash.
 */
void	*ft_memfind(char *hash)
{
	void	*ptr;

	ptr = NULL;
	if (!hash)
		return (NULL);
	if (find_block(g_alloc.zone[MEM_TINY], hash, ptr))
		return (ptr);
	if (find_block(g_alloc.zone[MEM_SMALL], hash, ptr))
		return (ptr);
	if (find_block(g_alloc.zone[MEM_LARGE], hash, ptr))
		return (ptr);
	return (NULL);
}
