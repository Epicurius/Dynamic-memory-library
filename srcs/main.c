/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 13:32:00 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

t_alloc	g_alloc =
{
	{NULL, NULL, NULL},
	PTHREAD_MUTEX_INITIALIZER,
	NULL,
	0
};

int main(void)
{
	char	*str[3];

	str[2] = ft_malloc(sizeof(char) * 200);
	show_alloc_mem();
	str[2] = ft_realloc1(str[2], sizeof(char) * 400);
	show_alloc_mem();
	printf("ft_free : %d\n", ft_free(str[0]));
	printf("ft_free : %d\n", ft_free(str[1]));
	printf("ft_free : %d\n", ft_free(str[2]));
	show_alloc_mem();
	return (0);
}
