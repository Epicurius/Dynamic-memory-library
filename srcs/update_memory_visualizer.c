/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 14/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "memory_visualizer.h"

void	blit_sizes(size_t size, char *type, int i)
{
	char		str[50];
	SDL_Surface	*tmp;
	SDL_Rect	dstr;

	size_to_str(str, size);
	tmp = TTF_RenderText_Blended(g_mem_vis.font, type,
			hex_to_sdl_color(0x87ff00));
	dstr = (SDL_Rect){FRAME_MARGIN * 2 + FRAME_WIDTH,
		FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * i, tmp->w, tmp->h};
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	dstr.x += tmp->w + 5;
	SDL_FreeSurface(tmp);
	dstr.w += tmp->w;
	dstr.h += tmp->h;
	tmp = TTF_RenderText_Blended(g_mem_vis.font, str,
			hex_to_sdl_color(0x87ff00));
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	SDL_FreeSurface(tmp);
	free(str);
}

void	blit_info(int i)
{
	char		str[50];
	SDL_Surface	*tmp;
	SDL_Rect	dstr;

	tmp = TTF_RenderText_Blended(g_mem_vis.font, VISUAIZER_INFO,
			hex_to_sdl_color(0x87ff00));
	dstr = (SDL_Rect){FRAME_MARGIN,
		FRAME_MARGIN + (FRAME_MARGIN + FRAME_HEIGHT) * i, tmp->w, tmp->h};
	SDL_BlitSurface(tmp, NULL, g_mem_vis.surface, &dstr);
	SDL_FreeSurface(tmp);
	free(str);
}

void	update_memory_visualizer_part2(int i)
{
	t_zone	*zone;
	size_t	size;

	zone = g_alloc.zone[MEM_LARGE];
	size = 0;
	while (zone) {
		size += ((t_block *)((void *)zone + sizeof(t_zone)))->size;
		zone = zone->next;
	}
	blit_sizes(size, "L:", ++i);
	blit_info(i);
	SDL_UpdateWindowSurface(g_mem_vis.win);
}

void	update_memory_visualizer(void)
{
	int		i;
	t_zone	*zone;
	size_t	size;

	SDL_memset(g_mem_vis.surface->pixels, 'A',
		g_mem_vis.surface->h * g_mem_vis.surface->pitch);
	i = -1;
	zone = g_alloc.zone[MEM_TINY];
	while (zone && ++i < BLOCKS_DISPLAYED) {
		draw_zone(zone, 0x00d75f, i, &size);
		blit_sizes(size, "T:", i);
		zone = zone->next;
	}
	zone = g_alloc.zone[MEM_SMALL];
	while (zone && ++i < (BLOCKS_DISPLAYED * 2)) {
		draw_zone(zone, 0x00ffff, i, &size);
		blit_sizes(size, "S:", i);
		zone = zone->next;
	}
	update_memory_visualizer_part2(i);
}
