/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 10/04/2023 Niklas Neronin
 * Updated: 12/04/2023 Niklas Neronin
 */

#include "libdm.h"

static int  fd;
static char *buffer;
static char *g_hextable = "0123456789abcdef";
static int  row_size = 2 + BYTES_PER_ROW * 3 + 3;

static void show_hex(const unsigned char *data, size_t size)
{
	char *ptr;
	char *buff = buffer;

	memset(buff, ' ', row_size);
	buff[0] = '\t';
	buff[1] = '[';
	buff[row_size - 2] = ']';
	buff[row_size - 1] = '\n';

	for (size_t i = 0; i < size; ) {
		/* Buffer hexadecimal representation. */
		size_t res = i + BYTES_PER_ROW;
		if (res > size) {
			memset(&buff[3], ' ', row_size - 6);
			for (ptr = &buff[3]; i < size; i++, ptr += 3) {
				ptr[0] = g_hextable[data[i] / 16];
				ptr[1] = g_hextable[data[i] % 16];
			}
			write(fd, buff, row_size);
			return ;
		}

		for (ptr = &buff[3]; i < res; i++, ptr += 3) {
			ptr[0] = g_hextable[data[i] / 16];
			ptr[1] = g_hextable[data[i] % 16];
		}
		write(fd, buff, row_size);
	}
}

static void	print_blocks(t_block *block, int flags)
{
	int	num, i;

	num = 0;
	while (block) {
		if (!block->free || (block->free && flags & MEM_SHOW_FREE)) {
			i = sprintf(buffer, "\t- BLOCK %d: %p - %p : %lu bytes", num++,
						(void *)block + sizeof(t_block),
						(void *)block + sizeof(t_block) + block->size,
						block->size);

			if (flags & MEM_SHOW_HASH && block->free == FALSE)
				i += sprintf(&buffer[i], " \x1b[38;5;118m[%.4s]\x1b[0m", block->str);

			if (block->free) {
				memcpy(&buffer[i], " \x1b[32mFREE\x1b[0m\n", 15);
				i += 15;
			}
			else {
				memcpy(&buffer[i], " \x1b[36mRESERVED\x1b[0m\n", 19);
				i += 19;
			}
			write(fd, buffer, i);

			if (flags & MEM_SHOW_HEX && block->free == FALSE)
				show_hex((unsigned char *)block + sizeof(t_block), block->size);
		}
		block = block->next;
	}
}

static void print_zones(t_zone *zone, int flags)
{
	int	i, num;

	num = 0;
	while (zone) {
		i = sprintf(buffer, "\tZONE %d : %lu bytes\n", num++,
					(zone->end - (void *)zone));
		write(fd, buffer, i);

		print_blocks((void *)zone + sizeof(t_zone), flags);
		zone = zone->next;
	}
}

void ft_memshow(int fd, int flags)
{
	size_t size;

	size = 256 > row_size ? 256 : row_size;
	buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
				  -1, 0);
	if (buffer == MAP_FAILED)
		ERROR("'mmap()' failed to allocated buffer in 'ft_memshow()'");

	if (!(flags & (MEM_SHOW_TINY | MEM_SHOW_SMALL | MEM_SHOW_LARGE)))
		flags |= MEM_SHOW_TINY | MEM_SHOW_SMALL | MEM_SHOW_LARGE;

	pthread_mutex_lock(&g_alloc.mutex);
	if (flags & MEM_SHOW_TINY && g_alloc.zone[MEM_TINY]) {
		write(fd, buffer, sprintf(buffer, "\x1b[38;5;41mTINY\x1b[0m   : %p\n",
								  g_alloc.zone[MEM_TINY]));

		print_zones(g_alloc.zone[MEM_TINY], flags);
	}
	if (flags & MEM_SHOW_SMALL && g_alloc.zone[MEM_SMALL]) {
		write(fd, buffer, sprintf(buffer, "\x1b[38;5;51mSMALL\x1b[0m  : %p\n",
								  g_alloc.zone[MEM_SMALL]));

		print_zones(g_alloc.zone[MEM_SMALL], flags);
	}
	if (flags & MEM_SHOW_LARGE && g_alloc.zone[MEM_LARGE]) {
		write(fd, buffer, sprintf(buffer, "\x1b[38;5;61mLARGE\x1b[0m  : %p\n",
								  g_alloc.zone[MEM_LARGE]));

		print_zones(g_alloc.zone[MEM_LARGE], flags);
	}
	pthread_mutex_unlock(&g_alloc.mutex);

	if (munmap(buffer, size))
		ERROR("'munmap()' failed to free buffer in 'ft_memshow()'");
}
