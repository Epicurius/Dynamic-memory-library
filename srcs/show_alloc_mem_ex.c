/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 14:37:33 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/17 10:37:52 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

static void	hexdump_row(unsigned char *c, size_t num)
{
	size_t	i;

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
	i = block->size;
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

static void	init_flags(int *fd, int *flags)
{
	if (*flags & MEM_WRITE)
		*fd = creat("./memdump.txt", S_IRUSR | S_IWUSR);
	else
		*fd = 1;
	if (!(*flags & MEM_SHOW_MEM_SMALL) && !(*flags & MEM_SHOW_MEM_LARGE)
		&& !(*flags & MEM_SHOW_MEM_TINY))
		*flags = MEM_SHOW_MEM_SMALL | MEM_SHOW_MEM_LARGE
			| MEM_SHOW_MEM_TINY | *flags;
}

void	show_alloc_mem_ex(int flags)
{
	int	fd;

	pthread_mutex_lock(&g_alloc.mutex);
	init_flags(&fd, &flags);
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
