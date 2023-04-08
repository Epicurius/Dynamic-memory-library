/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 12/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libmem.h"

static void	print_blocks(int fd, t_block *block, int flags)
{
	int	i;

	i = 0;
	while (block)
	{
		if (!block->free || (block->free && flags & MEM_SHOW_FREE))
		{
			ft_dprintf(fd, "\t- BLOCK %d: %p - %p : %lu bytes, ", i++,
				(void *)block + sizeof(t_block),
				(void *)block + sizeof(t_block) + block->size,
				block->size);
			if (flags & MEM_SHOW_HASH && block->free == FALSE)
				ft_dprintf(fd, "{CLR:118}[%.4s]{RESET} ", block->str);
			if (block->free)
				ft_dprintf(fd, "{GREEN}FREE{RESET}\n");
			else
				ft_dprintf(fd, "{CYAN}RESERVED{RESET}\n");
			if (flags & MEM_HEXDUMP && block->free == FALSE)
				hexdump(block);
		}
		block = block->next;
	}
}

void	print_zones(int fd, t_zone *zone, int flags)
{
	int	i;

	i = 0;
	while (zone)
	{
		ft_dprintf(fd, "\tZONE %d : %lu bytes\n", i++,
			(zone->end - (void *)zone));
		print_blocks(fd, (void *)zone + sizeof(t_zone), flags);
		zone = zone->next;
	}
}

void	show_alloc_mem(void)
{
	pthread_mutex_lock(&g_alloc.mutex);
	ft_printf("{CLR:41}TINY{RESET}  : %p\n", g_alloc.zone[MEM_TINY]);
	print_zones(1, g_alloc.zone[MEM_TINY], 0);
	ft_printf("{CLR:51}SMALL{RESET} : %p\n", g_alloc.zone[MEM_SMALL]);
	print_zones(1, g_alloc.zone[MEM_SMALL], 0);
	ft_printf("{CLR:61}LARGE{RESET} : %p\n", g_alloc.zone[MEM_LARGE]);
	print_zones(1, g_alloc.zone[MEM_LARGE], 0);
	pthread_mutex_unlock(&g_alloc.mutex);
}
