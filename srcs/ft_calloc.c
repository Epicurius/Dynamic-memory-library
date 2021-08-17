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
	else if (total <= MEM_TINY_MAX)
		mem = alloc_amount(MEM_TINY, MEM_TINY_sizeof(t_zone), total);
	else if (total <= MEM_SMALL_MAX)
		mem = alloc_amount(MEM_SMALL, MEM_SMALL_sizeof(t_zone), total);
	else
		mem = alloc_amount(MEM_LARGE, sizeof(t_block)
				+ sizeof(t_zone) + total, total);
	if (mem)
		ft_bzero(mem, total);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
