/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:37:33 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 15:48:25 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

static void	print_blocks(t_block *block)
{
	int	i;

	i = 0;
	while (block)
	{
		ft_printf("\t- BLOCK %d: %p - %p : %lu bytes, ", i,
			(void *)block + sizeof(t_block),
			(void *)block + sizeof(t_block) + block->memsize,
			block->memsize);
		if (block->free)
			ft_printf("{GREEN}YES{RESET}\n");
		else
			ft_printf("{RED}NO{RESET}\n");
		block = block->next;
		i++;
	}
}

static void	print_zones(t_zone *zone, int zone_id)
{
	int	i;

	i = 0;
	while (zone)
	{
		ft_printf("\tZONE %d : %lu bytes\n", i,
			(zone->end - (void *)zone));
		print_blocks((void *)zone + sizeof(t_zone));
		i++;
		zone = zone->next;
	}
}

void	show_alloc_mem(void)
{
	pthread_mutex_lock(&g_alloc.mutex);
	ft_printf("{CLR:41}TINY{RESET} : %p\n", g_alloc.zone[TINY]);
	print_zones(g_alloc.zone[TINY], TINY);
	ft_printf("{CLR:51}SMALL{RESET} : %p\n", g_alloc.zone[SMALL]);
	print_zones(g_alloc.zone[SMALL], SMALL);
	ft_printf("{CLR:61}LARGE{RESET} : %p\n", g_alloc.zone[LARGE]);
	print_zones(g_alloc.zone[LARGE], LARGE);
	pthread_mutex_unlock(&g_alloc.mutex);
}
