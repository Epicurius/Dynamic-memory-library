/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:37:33 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 14:42:16 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

#define ROW_SIZE	15

static void	hexdump_row(unsigned char *c, size_t num)
{
	size_t			i;

	ft_printf("\t   [");
	i = -1;
	while (++i < num)
	{
		ft_puthex(*(c + i));
		ft_putchar(' ');
	}
	while (++i <= ROW_SIZE)
		ft_putstr("   ");
	ft_printf("] [");
	i = -1;
	while (++i < num)
	{
		if (' ' <= *(c + i) && *(c + i) <= '~')
			ft_printf("{BOLD}%c{RESET}", *(c + i));
		else
			ft_putchar(' ');
		ft_putstr("  ");
	}
	while (++i <= ROW_SIZE)
		ft_putstr("   ");
	ft_printf("]\n");
}

void	hexdump(t_block *block)
{
	size_t			i;
	unsigned char	*c;

	c = (unsigned char *)block + sizeof(t_block);
	i = block->memsize;
	while (i > 0)
	{
		if (i > ROW_SIZE)
		{
			hexdump_row(c, ROW_SIZE);
			i -= ROW_SIZE;
		}
		else
		{
			hexdump_row(c, i);
			i = 0;
		}
		c += ROW_SIZE;
	}
}

static void	print_blocks(t_block *block, int flags)
{
	int	i;

	i = 0;
	while (block)
	{
		if (!block->free || (block->free && flags & MEM_SHOW_FREE))
		{
			ft_printf("\t- BLOCK %d: %p - %p : %lu bytes, ", i++,
				(void *)block + sizeof(t_block),
				(void *)block + sizeof(t_block) + block->memsize,
				block->memsize);
			if (block->free)
				ft_printf("{GREEN}FREE{RESET}\n");
			else
				ft_printf("{CYAN}RESERVED{RESET}\n");
			if (flags & MEM_HEXDUMP && block->free == FALSE)
				hexdump(block);
		}
		block = block->next;
	}
}

static void	print_zones(t_zone *zone, int zone_id, int flags)
{
	int	i;

	i = 0;
	while (zone)
	{
		ft_printf("\tZONE %d : %lu bytes\n", i++, (zone->end - (void *)zone));
		print_blocks((void *)zone + sizeof(t_zone), flags);
		zone = zone->next;
	}
}

void	show_alloc_mem(void)
{
	pthread_mutex_lock(&g_alloc.mutex);
	ft_printf("{CLR:41}TINY{RESET} : %p\n", g_alloc.zone[TINY]);
	print_zones(g_alloc.zone[TINY], TINY, 0);
	ft_printf("{CLR:51}SMALL{RESET} : %p\n", g_alloc.zone[SMALL]);
	print_zones(g_alloc.zone[SMALL], SMALL, 0);
	ft_printf("{CLR:61}LARGE{RESET} : %p\n", g_alloc.zone[LARGE]);
	print_zones(g_alloc.zone[LARGE], LARGE, 0);
	pthread_mutex_unlock(&g_alloc.mutex);
}


void	show_alloc_mem_ex(int flags)
{
	pthread_mutex_lock(&g_alloc.mutex);
	if (!(flags & MEM_SHOW_SMALL) && !(flags & MEM_SHOW_LARGE) && !(flags & MEM_SHOW_TINY))
		flags = MEM_SHOW_SMALL | MEM_SHOW_LARGE | MEM_SHOW_TINY | flags;
	if (flags & MEM_SHOW_TINY)
	{
		ft_printf("{CLR:41}TINY{RESET} : %p\n", g_alloc.zone[TINY]);
		print_zones(g_alloc.zone[TINY], TINY, flags);
	}
	if (flags & MEM_SHOW_SMALL)
	{
		ft_printf("{CLR:51}SMALL{RESET} : %p\n", g_alloc.zone[SMALL]);
		print_zones(g_alloc.zone[SMALL], SMALL, flags);
	}
	if (flags & MEM_SHOW_LARGE)
	{
		ft_printf("{CLR:61}LARGE{RESET} : %p\n", g_alloc.zone[LARGE]);
		print_zones(g_alloc.zone[LARGE], LARGE, flags);
	}
	pthread_mutex_unlock(&g_alloc.mutex);
}

