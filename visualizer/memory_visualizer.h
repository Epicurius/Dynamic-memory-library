/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 14/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#ifndef MEMORY_VISUALIZER_H
# define MEMORY_VISUALIZER_H

# include "./frameworks/SDL2.framework/Headers/SDL.h"
# include "./frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"
# include "libmem.h"
# include "libft.h"
# include "libpf.h"

# define FRAME_WIDTH		1100
# define FRAME_HEIGHT		20
# define FRAME_MARGIN		10
# define BLOCKS_DISPLAYED	5
# define W 1280
# define H 330

typedef struct s_point
{
	int						x;
	int						y;
}							t_point;

typedef struct s_memory_visualizer
{
	SDL_Window				*win;
	SDL_Surface				*surface;
	TTF_Font				*font;
}							t_memory_visualizer;

extern t_memory_visualizer	g_mem_vis;

void		update_memory_visualizer(void);
void		init_memory_visualizer(void);
void		free_memory_visualizer(void);

void		draw_zone(t_zone *zone, Uint32 color, int i, size_t *size);
void		draw_box(SDL_Surface *surface, Uint32 color,
				t_point upper_left, t_point dimen);
void		draw_sq(SDL_Surface *surface, Uint32 color,
				t_point pos, t_point dimen);
void		size_to_str(char *str, int nbr);
SDL_Color	hex_to_sdl_color(int hex);

#endif
