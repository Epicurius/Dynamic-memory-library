/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexdump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 18:51:58 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/15 18:53:39 by nneronin         ###   ########.fr       */
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
