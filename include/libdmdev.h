/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 * 
 * Created: Niklas Neronin <niklas.neronin@gmail.com>
 */

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

# include <libdm.h>

# define TRUE            1
# define FALSE           0

# define DEBUG(...)	{ \
	write(1, "\e[0;36mDEBUG\e[0m: ", 18); \
	write(1, g_libdm.debug, sprintf(g_libdm.debug, __VA_ARGS__)); \
	write(1, "\n", 1); \
}

# define ERROR(...) { \
	write(1, "\e[0;31mERROR\e[0m: ", 18); \
	write(1, g_libdm.debug, sprintf(g_libdm.debug, __VA_ARGS__)); \
	write(1, "\n", 1); \
	exit(1); \
}

enum zone_type
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
struct block
{
	struct block	*next;
	int				free;
	char			str[4];
	size_t			size;
};

/*
 * Size: 16 Bytes
 * 'next' - pointer to the next zone.
 * 'end'  - pointer to zone end.
 */
struct zone
{
	struct zone	*next;
	void		*end;
};

/*
 * Size: 16 Bytes
 * 'zone'  - head for each zone type.
 * 'mutex' - mutual exclusion object.
 * 'debug' - debug message buffer.
 */
struct libdm
{
	struct zone		*zone[3];
	pthread_mutex_t	mutex;
	char 			debug[256];
};

extern struct libdm g_libdm;

/* Non-thread safe versions */
void	*_malloc(size_t size);
void	_free(void *ptr);

/* Zone related */
struct zone		*new_zone(struct zone **head, size_t size);
size_t			get_zone_size(size_t size);
enum zone_type	get_zone_type(size_t size);

/* Block related */
size_t			get_alloc_size(void *ptr);
size_t			get_block_max(int type);
struct block	*split_block(struct block *block, size_t size);
struct block	*shrink_block(struct block *block, size_t size);
struct block	*downscale_block(struct block *block, size_t size, size_t min);
struct block	*combine_block(struct block *block, size_t size);
struct block	*grow_block(struct block *block, size_t size);
struct block	*upscale_block(struct block *block, size_t size, size_t min,
							   size_t max);
struct block	*resize_block(struct block *block, size_t size);
