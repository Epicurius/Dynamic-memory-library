/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_visualizer.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 08:36:27 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/14 14:53:28 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_VISUALIZER_H
# define MEMORY_VISUALIZER_H

#  include "../frameworks/SDL2.framework/Headers/SDL.h"
#  include "../frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"
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
	t_alloc					alloc;
}							t_memory_visualizer;

extern t_memory_visualizer	g_mem_vis;

int			update_memory_visualizer();
void		init_memory_visualizer();
void		free_memory_visualizer();

void		draw_zone(t_zone *zone, Uint32 color, int i, size_t *size);
void		draw_box(SDL_Surface *surface, Uint32 color, t_point upper_left, t_point dimen);
void		draw_sq(SDL_Surface *surface, Uint32 color, t_point pos, t_point dimen);
void		size_to_str(char *str, int nbr);
SDL_Color	hex_to_sdl_color(int hex);

#endif
