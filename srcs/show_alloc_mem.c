/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:37:33 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/16 18:10:14 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	print_zones(int fd, t_zone *zone, int flags)
{
	int	i;

	i = 0;
	while (zone)
	{
		ft_dprintf(fd, "\tZONE %d : %lu bytes\n", i++, (zone->end - (void *)zone));
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

/*
 *	TODO: Memory sum.
 */
void	show_alloc_mem_ex(int flags)
{
	int	fd;
	pthread_mutex_lock(&g_alloc.mutex);
	if (flags & MEM_WRITE)
		fd = creat("./memdump.txt", S_IRUSR | S_IWUSR);
	else
		fd = 1;
		
	if (!(flags & MEM_SHOW_MEM_SMALL) && !(flags & MEM_SHOW_MEM_LARGE)
		&& !(flags & MEM_SHOW_MEM_TINY))
		flags = MEM_SHOW_MEM_SMALL | MEM_SHOW_MEM_LARGE | MEM_SHOW_MEM_TINY | flags;
	if (flags & MEM_SHOW_MEM_TINY)
	{
		ft_dprintf(fd, "{CLR:41}TINY{RESET}  : %p\n", g_alloc.zone[MEM_TINY]);
		print_zones(fd, g_alloc.zone[MEM_TINY], flags);
	}
	if (flags & MEM_SHOW_MEM_SMALL)
	{
		ft_dprintf(fd, "{CLR:51}SMALL{RESET} : %p\n", g_alloc.zone[MEM_SMALL]);
		print_zones(fd, g_alloc.zone[MEM_SMALL], flags);
	}
	if (flags & MEM_SHOW_MEM_LARGE)
	{
		ft_dprintf(fd, "{CLR:61}LARGE{RESET} : %p\n", g_alloc.zone[MEM_LARGE]);
		print_zones(fd, g_alloc.zone[MEM_LARGE], flags);
	}
	if (flags & MEM_WRITE)
		close(fd);
	pthread_mutex_unlock(&g_alloc.mutex);
}
