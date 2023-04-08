/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 13/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libmem.h"

/*
 *	Same as malloc except it sets all bytes to 0.
 *	More info in malloc.
 */
void	*calloc(size_t num, size_t size)
{
	void	*mem;
	size_t	total;

	pthread_mutex_lock(&g_alloc.mutex);
	total = num * size;
	if (total <= 0)
		mem = NULL;
	else if (total <= MEM_TINY_MAX)
		mem = alloc_amount(MEM_TINY, MEM_TINY_ZONE, total);
	else if (total <= MEM_SMALL_MAX)
		mem = alloc_amount(MEM_SMALL, MEM_SMALL_ZONE, total);
	else
		mem = alloc_amount(MEM_LARGE, sizeof(t_block)
				+ sizeof(t_zone) + total, total);
	if (mem)
		ft_bzero(mem, total);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
