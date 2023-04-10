/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 10/04/2023 Niklas Neronin
 * Updated: 10/04/2023 Niklas Neronin
 */


#include "libdm.h"

int	main(void)
{
	char *arr[4];

	arr[0] = ft_malloc(10, "HEH");
	arr[1] = calloc(1, 98);
	memcpy(arr[1], "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUV" \
				   "WXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\n\t\0", 97);
	arr[1] = malloc(400);
	arr[1] = realloc(arr[1], 300);
	arr[2] = ft_malloc(5, "arr");
	arr[3] = malloc(2048);
	free(arr[3]);
	arr[3] = malloc(4096);
	ft_mempurge();
	return 1;

}
