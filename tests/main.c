/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/15 10:31:19 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
#include "libmem.h"
int main(void)
{
	char	*str[1000];

	int i = -1;

	init_memory_visualizer();
	while (++i < 1000)
	{
		str[i] = malloc(sizeof(char) * (rand() % 1023));
		update_memory_visualizer();
		//usleep(10000);
	}
	int j;
	while (--i >= 0)
	{
		//j = rand() % 1000;
		//if (str[j])
		//	free(str[j]);
		free(str[i]);
		update_memory_visualizer();
	}
	sleep(2);
	free_memory_visualizer();
	system("leaks a.out");
	return (0);
}
