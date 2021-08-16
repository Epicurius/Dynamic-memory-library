/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 10:57:08 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/16 11:20:48 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

void	copy_malloc_hash(t_block *block, char *hash)
{
	int	i;
	int len;
	
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
	else if (size <= TINY_MAX)
		mem = alloc_amount(TINY, TINY_ZONE_SIZE, size);
	else if (size <= SMALL_MAX)
		mem = alloc_amount(SMALL, SMALL_ZONE_SIZE, size);
	else
		mem = alloc_amount(LARGE, BLOCK_SIZE + ZONE_SIZE + size, size);
	if (!mem)
		exit(1);
	copy_malloc_hash((void *)mem - sizeof(t_block), hash);
	pthread_mutex_unlock(&g_alloc.mutex);
	return (mem);
}
