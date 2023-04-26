/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include "libdm.h"

/*
 * Same as 'malloc()' except it sets all bytes to 'NULL'.
 */
void *calloc(size_t num, size_t size)
{
	void	*mem;
	size_t	total;

	total = num * size;
	if (total <= 0)
		return NULL;

	pthread_mutex_lock(&g_libdm.mutex);
	mem = _malloc(total);
	if (mem)
		memset(mem, 0, total);
	pthread_mutex_unlock(&g_libdm.mutex);

	return mem;
}
