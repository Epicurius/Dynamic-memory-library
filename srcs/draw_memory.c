/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 14:52:39 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/16 09:20:42 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_visualizer.h"

int	change(size_t x, size_t min, size_t max)
{
	return ((FRAME_WIDTH * (x - min) / (max - min)));
}

/*
 *	Multiply block->size to get more color variation.
 */
static void	draw_block(t_block *block, t_zone *zone, int index)
{
	int	y_top;
	int	x_start;
	int	x_len;

	y_top = FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * index;
	x_start = FRAME_MARGIN + change((size_t)block,
			(size_t)zone, (size_t)zone->end);
	if (block->next)
		x_len = FRAME_MARGIN + change((size_t)block->next,
				(size_t)zone, (size_t)zone->end) - x_start;
	else
		x_len = FRAME_WIDTH - x_start;
	x_len = ft_max(x_len, 1);
	draw_sq(g_mem_vis.surface, block->size,
		(t_point){x_start, y_top}, (t_point){x_len, FRAME_HEIGHT});
}

void	draw_zone(t_zone *zone, Uint32 color, int i, size_t *size)
{
	int		y_top;
	t_block	*block;

	*size = 0;
	y_top = FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * i;
	block = (void *)zone + sizeof(t_zone);
	while (block)
	{
		if (block->free == FALSE)
		{
			*size += block->size;
			draw_block(block, zone, i);
		}
		block = block->next;
	}
	draw_box(g_mem_vis.surface, color,
		(t_point){FRAME_MARGIN, y_top}, (t_point){FRAME_WIDTH, FRAME_HEIGHT});
}
