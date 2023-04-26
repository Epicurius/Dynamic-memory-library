/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 *	Same as malloc except it sets all bytes to 0.
 *	More info in malloc.
 */
void	*calloc(size_t num, size_t size)
{
	void	*mem;
	size_t	total;

	total = num * size;
	if (total <= 0)
		return NULL;

	pthread_mutex_lock(&g_alloc.mutex);
	mem = _malloc(total);
	if (mem)
		memset(mem, 0, total);
	pthread_mutex_unlock(&g_alloc.mutex);

	return mem;
}
