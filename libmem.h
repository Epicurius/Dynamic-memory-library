/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 09:04:42 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/12 11:48:48 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <errno.h>
# include <pthread.h>
#include "stdio.h"
#include "libpf.h"


#define TRUE	1
#define FALSE	0

enum				e_zone
{
	TINY = 0,
	SMALL = 1,
	LARGE = 2,
	ALL = 3,
};

/*
 *	printf("%lu %lu %d\n", sizeof(t_zone), sizeof(t_block), getpagesize());
 *		= 16 32 4096
 *	(128 + 32) * 100 + 16 = 16016 bytes
 *	(1024 + 32) * 100 + 16 = 105616 bytes
 *	The zone size must be larger.
 *	TODO: adjust zone sizes until optimal amount.
 *	Each zone must contain at least 100 allocations.
 */
# define BLOCK_SIZE			sizeof(t_block)
# define ZONE_SIZE			sizeof(t_zone)
# define TINY_MAX			128
# define TINY_ZONE_SIZE		4096 * 4
# define SMALL_MAX			1024
# define SMALL_ZONE_SIZE	4096 * 26

/*
 *	Size: 32 Bytes
 *	Addres to the next block to the right.
 *	Is free or reserved.
 *	The amount of user data.
 *	User data + infastructure data size
 */
typedef struct		s_block
{
	struct s_block	*next;
	int				free;
	size_t			memsize;
	size_t			checksum; //Maybe remove and let visualizer calculate.
}					t_block;

/*
 *	Size: 16 Bytes
 *	Next same type zeon address.
 *	Address to the end of current zone.
 */
typedef struct		s_zone
{
	struct s_zone	*next;
	void			*end;
}					t_zone;

typedef struct s_alloc
{
	t_zone			*zone[3];
	pthread_mutex_t	mutex;
	pthread_t		main_prog;
	int				debug;
}					t_alloc;

extern t_alloc		g_alloc;

void	show_alloc_mem(void);
void	*alloc_amount(int type, size_t total, size_t memsize);
void	update_next_block(t_zone *zone, t_block *block);
void	*create_new_zone(t_zone **head, size_t size);
void	*ft_malloc(size_t memsize);
void	*ft_calloc(size_t num, size_t size);
void	*ft_realloc1(void *ptr, size_t size);
int		ft_free(void *ptr);

#endif
