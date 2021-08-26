/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 10:57:08 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/26 16:14:44 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

void	copy_malloc_hash(t_block *block, char *hash)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(hash);
	while (++i < ft_min(4, len))
		block->str[i] = hash[i];
}

void	*ft_malloc(size_t size, char *hash)
{
	void	*mem;

	pthread_mutex_lock(&g_alloc.mutex);
	if (size <= 0)
		mem = NULL;
	else if (size <= MEM_TINY_MAX)
		mem = alloc_amount(MEM_TINY, MEM_TINY_ZONE, size);
	else if (size <= MEM_SMALL_MAX)
		mem = alloc_amount(MEM_SMALL, MEM_SMALL_ZONE, size);
	else
		mem = alloc_amount(MEM_LARGE, sizeof(t_block)
				+ sizeof(t_zone) + size, size);
	if (!mem)
	{
		ft_printf("{RED}[ERROR]{RESET} ft_malloc: %s\n", hash);
		exit(1);
	}
	if (hash)
		copy_malloc_hash((void *)mem - sizeof(t_block), hash);
	ft_bzero(mem, size);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
