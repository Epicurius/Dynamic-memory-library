/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 14/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "memory_visualizer.h"

static void	put_pixel(SDL_Surface *surface, Uint32 color, int x, int y)
{
	((Uint32 *)surface->pixels)[y * surface->w + x] = color;
}

static void	draw_hline(SDL_Surface *surface, Uint32 color, t_point pos, int len)
{
	while (--len > -1)
		put_pixel(surface, color, pos.x + len, pos.y);
}

static void	draw_vline(SDL_Surface *surface, Uint32 color, t_point pos, int len)
{
	while (--len > -1)
		put_pixel(surface, color, pos.x, pos.y + len);
}

void	draw_box(SDL_Surface *surface, Uint32 color,
	t_point upper_left, t_point dimen)
{
	t_point	upper_right;
	t_point	bottom_left;

	upper_right.x = upper_left.x + dimen.x - 1;
	upper_right.y = upper_left.y;
	bottom_left.x = upper_left.x;
	bottom_left.y = upper_left.y + dimen.y - 1;
	draw_hline(surface, color, upper_left, dimen.x);
	draw_hline(surface, color, bottom_left, dimen.x);
	draw_vline(surface, color, upper_right, dimen.y);
	draw_vline(surface, color, upper_left, dimen.y);
}

void	draw_sq(SDL_Surface *surface, Uint32 color, t_point pos, t_point dimen)
{
	int			y;

	y = 0;
	while (y < dimen.y)
	{
		draw_hline(surface, color, pos, dimen.x);
		pos.y++;
		y++;
	}
}
