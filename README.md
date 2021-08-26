## Malloc
	A dynamic allocation memory management library in c.
	It can be used in programs already in use without modifying them or recompiling.
	There are 3 classes of memory tiny, small and large.
	|---------------|---------------|---------------|
	|Type			|	Min Bytes	|	Max Bytes	|
	|---------------|---------------|---------------|
	|	TINY		|	0			|		128		|
	|	SMALL		|	128			|		1024	|
	|	LARGE		|	1024		|		INF		|
	|---------------|---------------|---------------|
	Tiny and small allocate a set size to save memory and speed up future tiny and small allocations.
	Large allocates the exact amount needed.
	128
	Each alloctaion is sizeof + 32 bytes and + 16 bytes if large or first tiny or small.

---
####	Functions that mimic existing functions.
	void	*malloc(size_t size)
		- Allocates size_t size amount of memory.
	void	*calloc(size_t num, size_t size)
		- Allocates size_t size amount of memory ans sets num amount to null.
	void	*realloc(void *ptr, size_t size)
		- Changes the memory pointed by ptr to size without loosing what it saved in it.
	void	free(void *ptr)
		- Frees the memory pointed to by *ptr.
---
####	Personal help functions
	void	*ft_malloc(size_t size, char *code)
		- Allocates size_t size amount of memory and saves code for debugging.
	void	*ft_memfind(char *hash)
		- Find pointer to memory with hash.
	void	ft_mempurge(void)
		- Delets all memory that was allocated.
	void	show_alloc_mem_ex(int flags)
		- Print usefull info to stdout.
		-----------------------------------------------------------------------
		MEM_SHOW_MEM_TINY	=	Print all TINY memory allocations.	
		MEM_SHOW_MEM_SMALL	=	Print all SMALL memory allocations.
		MEM_SHOW_MEM_LARGE	=	Print all LARGE memory allocations.
		MEM_HEXDUMP		=	Print all memory hex positions.	
		MEM_SHOW_FREE		=	Print all memory slots that are free.
		MEM_SHOW_HASH		=	Print all hash. (Use ft_malloc)	
		MEM_WRITE		=	Write all output to file.			
		-----------------------------------------------------------------------

---
###		Visualizer functions
	void	init_memory_visualizer(void);
	-	Init visualizer, has to be called once at program start.
	void	update_memory_visualizer(void);
	-	Each time is called redraws all the memomry.
	void	free_memory_visualizer(void);
	-	Frees everything allocated by the visualizer.
---
