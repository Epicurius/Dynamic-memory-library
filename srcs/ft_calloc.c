/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 11:22:57 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 13:20:13 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	else if (total <= TINY_MAX)
		mem = alloc_amount(TINY, TINY_ZONE_SIZE, total);
	else if (total <= SMALL_MAX)
		mem = alloc_amount(SMALL, SMALL_ZONE_SIZE, total);
	else
		mem = alloc_amount(LARGE, BLOCK_SIZE + ZONE_SIZE + total, total);
	if (mem)
		ft_bzero(mem, total);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
