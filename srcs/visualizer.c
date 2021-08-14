/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 07:57:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/14 15:06:30 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_visualizer.h"

t_memory_visualizer	g_mem_vis = {NULL, NULL};

SDL_Color	hex_to_sdl_color(int hex)
{
	SDL_Color	color;

	color.a = hex >> 24;
	color.r = hex >> 16;
	color.g = hex >> 8;
	color.b = hex;
	return (color);
}

void	init_sdl(t_memory_visualizer *vis)
{
	if (SDL_Init(SDL_INIT_VIDEO))
		error_msg("Could not init SDL: %s\n", SDL_GetError());
	if (TTF_Init())
		error_msg("Could not init TTF: %s\n", SDL_GetError());
	vis->win = SDL_CreateWindow("Memory Visualizer", 0, 0, W, H, 0);
	if (!vis->win)
		error_msg("Could not create window: %s\n", SDL_GetError());
	vis->surface = SDL_GetWindowSurface(vis->win);
	if (!vis->surface)
		error_msg("Could not create surface: %s\n", SDL_GetError());
}

void	init_memory_visualizer(void)
{
	SDL_Event	e;

	init_sdl(&g_mem_vis);
	SDL_PollEvent(&e);
}

void	free_memory_visualizer(void)
{
	SDL_FreeSurface(g_mem_vis.surface);
	SDL_DestroyWindow(g_mem_vis.win);
	SDL_Quit();
	TTF_Quit();
}
