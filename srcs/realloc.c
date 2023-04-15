/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 13/08/2021 Niklas Neronin
 * Updated: 15/04/2023 Niklas Neronin
 */

#include "libdm.h"

/*
 *	Loop through all the same type zones and there blocks until where ptr
 *	points to is found.
 */
static int find_at_zone(t_zone *zone, void *ptr, t_block **ptr_block,
						t_zone **ptr_zone)
{
	t_block	*block;

	while (zone) {
		if ((void *)zone < ptr && ptr < zone->end) {
			block = (void *)zone + sizeof(t_zone);
			while (block) {
				if ((void *)block + sizeof(t_block) == ptr) {
					*ptr_block = block;
					*ptr_zone = zone;
					return 1;
				}
				block = block->next;
			}
		}
		zone = zone->next;
	}
	return 0;
}

/*
 *	Find the block and zone where ptr points to.
 */
static int	find_block_and_zone(void *ptr, t_block **block, t_zone **zone)
{
	for (int i = MEM_TINY; i <= MEM_LARGE; i++) {
		if (find_at_zone(g_alloc.zone[i], ptr, block, zone))
			return TRUE;
	}
	return FALSE;
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

	new = NULL;
	pthread_mutex_lock(&g_alloc.mutex);
	if (find_block_and_zone(ptr, &block, &zone)) {
		if (!size)
			_free(ptr);
		else if (size <= block->size) {
			resize_block(block, size);
			new = ptr;
		}
		else {
			new = _malloc(size);
			if (new) {
				memcpy(new, ptr, block->size < size ? block->size : size);
				_free(ptr);
			}
		}
	}
	pthread_mutex_unlock(&g_alloc.mutex);
	return (new);
}
