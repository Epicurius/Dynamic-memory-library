/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mempurge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:14:51 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/17 08:29:30 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

static void	loop_zones(int type)
{
	t_zone	*zone;
	t_zone	*curr;

	zone = g_alloc.zone[type];
	while (zone)
	{
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
