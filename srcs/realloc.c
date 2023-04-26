/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 * Move the data to another block.
 */
static void *move_data(void *ptr, size_t size)
{
	void *new;

	new = _malloc(size);
	if (new) {
		t_block *block = ptr - sizeof(t_block);
		memcpy(new, ptr, block->size < size ? block->size : size);
		_free(ptr);
	}
	return new;
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
	t_block	*block;

	if (!ptr)
		return (malloc(size));

	new = NULL;
	pthread_mutex_lock(&g_alloc.mutex);
	if (!size)
		_free(ptr);
	else {
		block = resize_block(ptr - sizeof(t_block), size);
		if (!block)
			new = move_data(ptr, size);
		else
			new = (void *)block + sizeof(t_block);
	}
	pthread_mutex_unlock(&g_alloc.mutex);
	return (new);
}
