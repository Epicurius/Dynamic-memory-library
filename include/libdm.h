/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#ifndef LIBMEM_H
# define LIBMEM_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>
# include <pthread.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>

# define MEM_TINY_MAX    128
# define MEM_TINY_ZONE   16384
# define MEM_SMALL_MAX   1024
# define MEM_SMALL_ZONE  106496
# define BYTES_PER_ROW   20

# define TRUE            1
# define FALSE           0

# define DEBUG(...)	{ \
	write(1, "\e[0;36mDEBUG\e[0m: ", 18); \
	write(1, g_alloc.debug, sprintf(g_alloc.debug, __VA_ARGS__)); \
	write(1, "\n", 1); \
}

# define ERROR(...) { \
	write(1, "\e[0;31mERROR\e[0m: ", 18); \
	write(1, g_alloc.debug, sprintf(g_alloc.debug, __VA_ARGS__)); \
	write(1, "\n", 1); \
	exit(1); \
}

enum	e_mem_zone
{
	MEM_TINY,
	MEM_SMALL,
	MEM_LARGE
};

# define MEM_SHOW_TINY  0x00000001
# define MEM_SHOW_SMALL 0x00000010
# define MEM_SHOW_LARGE 0x00000100
# define MEM_SHOW_HEX   0x00001000
# define MEM_SHOW_CHAR  0x00010000
# define MEM_SHOW_FREE  0x00100000
# define MEM_SHOW_HASH  0x01000000

/*
 * Size: 24 Bytes
 * 'next' - pointer to the next block.
 * 'free' - free or reserved.
 * 'str'  - user designated hash.
 * 'size' - size of user allocated data.
 */
typedef struct s_block
{
	struct s_block	*next;
	int				free;
	char			str[4];
	size_t			size;
}					t_block;

/*
 * Size: 16 Bytes
 * 'next' - pointer to the next zone.
 * 'end'  - pointer to zone end.
 */
typedef struct s_zone
{
	struct s_zone	*next;
	void			*end;
}					t_zone;

/*
 * Size: 16 Bytes
 * 'zone'  - head for each zone type.
 * 'mutex' - mutual exclusion object.
 * 'debug' - debug message buffer.
 */
typedef struct s_alloc
{
	t_zone			*zone[3];
	pthread_mutex_t	mutex;
	char 			debug[256];
}					t_alloc;

extern t_alloc		g_alloc;

/* Public standard functions */
void	*malloc(size_t size);
void	*calloc(size_t num, size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);

/* Public custom functions */
void	ft_memshow(int fd, int flags);
void	*ft_malloc(size_t size, char *code);
void	*ft_memfind(char *hash);
void	ft_mempurge(void);

/* Private functions */
void	*_malloc(size_t size);
void	_free(void *ptr);
void	resize_block(t_block *block, size_t size);
void	*allocate_zone(t_zone **head, size_t size);

#endif
