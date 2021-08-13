/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:17:10 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/13 16:53:47 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"

int main(void)
{
	char	*str[3];

	str[2] = malloc(sizeof(char) * 200);
	//show_alloc_mem();
	str[2] = realloc(str[2], sizeof(char) * 400);
	//ft_strcpy(str[2], "Hello");
	show_alloc_mem_ex(MEM_HEXDUMP | MEM_SHOW_FREE);
	free(str[0]);
	free(str[1]);
	free(str[2]);
	show_alloc_mem();
	return (0);
}
