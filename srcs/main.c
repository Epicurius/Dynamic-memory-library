/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 14:55:48 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

t_alloc	g_alloc =
{
	{NULL, NULL, NULL},
	PTHREAD_MUTEX_INITIALIZER
};

int main(void)
{
	char	*str[3];

	str[2] = ft_malloc(sizeof(char) * 200);
	//show_alloc_mem();
	str[2] = ft_realloc1(str[2], sizeof(char) * 400);
	ft_strcpy(str[2], "Hello");
	show_alloc_mem_ex(MEM_HEXDUMP | MEM_SHOW_FREE);
	printf("ft_free : %d\n", ft_free(str[0]));
	printf("ft_free : %d\n", ft_free(str[1]));
	printf("ft_free : %d\n", ft_free(str[2]));
	show_alloc_mem();
	return (0);
}
