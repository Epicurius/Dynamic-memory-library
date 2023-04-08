/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 16/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

#include "libdm.h"

static void	loop_zones(int type)
{
	t_zone	*zone;
	t_zone	*curr;

	zone = g_alloc.zone[type];
	while (zone) {
		curr = zone;
		zone = zone->next;
		munmap(curr, (void *)curr->end - (void *)curr);
	}
	g_alloc.zone[type] = NULL;
}

void	ft_mempurge(void)
{
	pthread_mutex_lock(&g_alloc.mutex);
	loop_zones(MEM_TINY);
	loop_zones(MEM_SMALL);
	loop_zones(MEM_LARGE);
	pthread_mutex_unlock(&g_alloc.mutex);
}
