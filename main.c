/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 15:57:43 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

t_alloc					g_alloc =
{
	{NULL, NULL, NULL},
	PTHREAD_MUTEX_INITIALIZER,
	NULL,
	0
};

int main(void)
{
	//printf("%lu %lu %d\n", sizeof(t_zone), sizeof(t_block), getpagesize());
	char *str[3];
	str[0] = ft_malloc(sizeof(char) * 1);
	str[1] = ft_malloc(sizeof(char) * 84);
	str[2] = ft_malloc(sizeof(char) * 42949);
	show_alloc_mem();
	printf("ft_free : %d\n", ft_free(str[0]));
	printf("ft_free : %d\n", ft_free(str[1]));
	printf("ft_free : %d\n", ft_free(str[2]));
	show_alloc_mem();
	return (0);
}
