/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#ifndef LIBMEM_H
# define LIBMEM_H

# define MEM_TINY_MAX    256
# define MEM_SMALL_MAX   4096
# define BYTES_PER_ROW   20
# define BLOCKS_PER_ZONE 100
# define PAGE_SIZE       getpagesize()

/* Public standard functions */
void	*malloc(size_t size);
void	*calloc(size_t num, size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);

/* Public custom functions */
void	ft_memshow(int fd, int flags);
void	ft_mempurge(void);
void	*ft_malloc(size_t size, char *code);
void	*ft_memfind(char *hash);

#endif
