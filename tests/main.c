/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/16 18:17:26 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
#include "libmem.h"

int main(void)
{
	char	*str[1000];
	char	hash[5];

	int i = -1;

	init_memory_visualizer();
	while (++i < 1000)
	{
		//str[0] = ft_malloc(10, "0000");
		//str[1] = ft_malloc(10, "0001");
		//str[2] = ft_malloc(10, "0002");
		//str[3] = ft_malloc(10, "TEST");
		//show_alloc_mem_ex(MEM_SHOW_HASH | MEM_SHOW_FREE);
		//free(str[2]);
		//free(str[0]);
		///free(str[3]);
		//free(str[1]);
		size_to_str(hash, i);
		//str[i] = ft_malloc(rand() % 2000, hash);
		str[i] = ft_malloc((rand() % 1000) + 1, hash);
		update_memory_visualizer();
		//usleep(10000);
	}
	int j;
	while (--i >= 0)
	{
		j = rand() % 1000;
		if (str[j])
			free(str[j]);
		//free(str[i]);
		update_memory_visualizer();
		//usleep(10000);
	}
	
	i = -1;
	while (++i < 1000)
	{
		size_to_str(hash, i);
		str[i] = ft_malloc((rand() % 1000) + 1, hash);
		update_memory_visualizer();
		//usleep(10000);
	}
	while (--i >= 0)
	{
		j = rand() % 1000;
		if (str[j])
			free(str[j]);
		update_memory_visualizer();
		//usleep(10000);
	}
	//sleep(1);
	show_alloc_mem_ex(MEM_SHOW_HASH | MEM_SHOW_FREE | MEM_WRITE);
	ft_mempurge();
	//show_alloc_mem_ex(MEM_SHOW_HASH | MEM_SHOW_FREE | MEM_WRITE);
	update_memory_visualizer();
	usleep(30000);
	free_memory_visualizer();
	system("leaks a.out");
	return (0);
}
