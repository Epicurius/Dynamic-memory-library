/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 08/12/2021 Niklas Neronin
 * Updated: 12/04/2023 Niklas Neronin
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

/*
 *	Hive x64
 *	printf("%lu %lu %d\n", sizeof(t_zone), sizeof(t_block), getpagesize());
 *		= 16 24 4096
 *	(128 + 24) * 100 + 16 = 15216  < 4096 * 4
 *	(1024 + 24) * 100 + 16 = 105616 < 4096 * 26
 *	The zone size must be larger.
 *	TODO: adjust zone sizes until optimal amount.
 *	Each zone must contain at least 100 allocations.
 */
# define MEM_TINY_MAX    128
# define MEM_TINY_ZONE   16384
# define MEM_SMALL_MAX   1024
# define MEM_SMALL_ZONE  106496

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

/*
 *	Size: 24 Bytes
 *	Address to the next block to the right.
 *	Is free or reserved.
 *	The amount of user data.
 *	User data + infrastructure data size.
 *	str = 14,950 will work on x32 system.
 */
typedef struct s_block
{
	struct s_block	*next;
	int				free;
	char			str[4];
	size_t			size;
}					t_block;

/*
 *	Size: 16 Bytes
 *	Address to the next same type zone.
 *	Address to the end of current zone.
 */
typedef struct s_zone
{
	struct s_zone	*next;
	void			*end;
}					t_zone;

typedef struct s_alloc
{
	t_zone			*zone[3];
	pthread_mutex_t	mutex;
	char 			debug[256];
}					t_alloc;

extern t_alloc		g_alloc;

void	*malloc(size_t size);
void	*calloc(size_t num, size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);

void	*_malloc(size_t size);
void	_free(void *ptr);
void	*alloc_amount(int type, size_t total, size_t size);
void	update_next_block(t_zone *zone, t_block *block);
void	*create_new_zone(t_zone **head, size_t size);

void	*ft_malloc(size_t size, char *code);
void	*ft_memfind(char *hash);
void	ft_mempurge(void);

#endif
