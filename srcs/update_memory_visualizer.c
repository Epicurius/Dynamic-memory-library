/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_memory_visualizer.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 11:25:31 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/15 10:43:23 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_visualizer.h"

void	blit_sizes(size_t size, char *type, int i)
{
	char		str[50];
	SDL_Surface	*tmp;
	SDL_Rect	dstr;
	TTF_Font	*font;

	font = TTF_OpenFont("/Users/nneronin/Desktop/malloc/visualizer/Digital.ttf", 20);
	if (!font)
		error_msg("Could not open font: %s\n", TTF_GetError());
	size_to_str(str, size);
	tmp = TTF_RenderText_Blended(font, type, hex_to_sdl_color(0xff0f0f0f));
	dstr = (SDL_Rect){FRAME_MARGIN * 2 + FRAME_WIDTH,
		FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * i, tmp->w, tmp->h};
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	dstr.x += tmp->w + 5;
	SDL_FreeSurface(tmp);
	dstr.w += tmp->w;
	dstr.h += tmp->h;
	tmp = TTF_RenderText_Blended(font, str, hex_to_sdl_color(0xff0f0f0f));
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	SDL_FreeSurface(tmp);
	TTF_CloseFont(font);
	free(str);
}

void	blit_info(int i)
{
	char		str[50];
	SDL_Surface	*tmp;
	SDL_Rect	dstr;
	TTF_Font	*font;

	font = TTF_OpenFont("/Users/nneronin/Desktop/malloc/visualizer/Digital.ttf", 20);
	if (!font)
		error_msg("Could not open font: %s\n", TTF_GetError());
	tmp = TTF_RenderText_Blended(font, VISUAIZER_INFO,
			hex_to_sdl_color(0xff0f0f0f));
	dstr = (SDL_Rect){FRAME_MARGIN,
		FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * i, tmp->w, tmp->h};
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	SDL_FreeSurface(tmp);
	TTF_CloseFont(font);
	free(str);
}

int	update_memory_visualizer(void)
{
	int		i;
	t_zone	*zone;
	size_t	size;

	SDL_memset(g_mem_vis.surface->pixels, 'A',
		g_mem_vis.surface->h * g_mem_vis.surface->pitch);
	i = -1;
	zone = g_alloc.zone[TINY];
	while (zone && ++i < BLOCKS_DISPLAYED)
	{
		draw_zone(zone, 0x00d75f, i, &size);
		blit_sizes(size, "T:", i);
		zone = zone->next;
	}
	zone = g_alloc.zone[SMALL];
	while (zone && ++i < (BLOCKS_DISPLAYED * 2))
	{
		draw_zone(zone, 0x00ffff, i, &size);
		blit_sizes(size, "S:", i);
		zone = zone->next;
	}
	zone = g_alloc.zone[LARGE];
	size = 0;
	while (zone)
	{
		size += ((t_block *)((void *)zone + sizeof(t_zone)))->memsize;
		zone = zone->next;
	}
	blit_sizes(size, "L:", ++i);
	blit_info(i);
	SDL_UpdateWindowSurface(g_mem_vis.win);
	return (1);
}
