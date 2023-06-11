
##	My dynamic memory allocation & management library in C.

#### Summary

* [Installation](#installation)
* [Explanation](#explanation)
* [Notes](#notes)
* [Recreated Functions](#recreated-functions)
* [Custom Functions](#custom-functions)

<img src="./Images/Visualizer.gif" alt="drawing" width="800"/>

## Installation

```sh
git clone https://github.com/Epicurius/Dynamic-memory-library.git libdm
cd libdm
make
```
---

## Explanation

### **Block**
For each allocation the library saves some metadata in 'struct block'.
```c
struct block
{
	struct block 	 *next;
	int		 free;
	char		 str[4];
	size_t		 size;
};
```
|Variable				|Explanation												|Bytes	|
|---					|---														|:---:	|
|struct block *next		|Pointer to the next block, NULL if is the last one.		|	8	|
|int free				|Bool for whether the block is free or reserved.			|4		|
|char str[4]			|Utilized by 'ft_malloc()' to save hash.	                |1 * 4	|
|size_t size			|Size of the memory stored.									|8		|

So every time an allocation is requested the total occupied memory is __requested size + sizeof(struct block)__.<br>

### **Zone**

Each block is contained in a zone, zones are a large block of memory that act like a buffer from which
we can distribute smaller chunks of memory on request.

For example, 100 'malloc(5)' calls will request memory 100 times from the operating system, which is very slow.
Instead we request a larger chunk of memory at once and divvy it up ourselves, 'struct zone' is used to store the
zones memory's boundaries.

```c
struct zone
{
	struct zone	*next;
	void		*end;
};
```
|Variable				|Explanation												|Bytes	|
|---					|---														|:---:	|
|struct s_zone *next	|Pointer to the next zone, NULL if is the last one.			|	8	|
|void *end				|Pointer to the the end of the zone.						|	8	|

There are 3 zone types; TINY, SMALL and LARGE.<br>
TINY/SMALL zone size is the lowest possible amount of __PAGE_SIZE__ that can fit the minimum amount of blocks:<br>
__(sizeof(struct block) + avg. block size) * BLOCKS_PER_ZONE + sizeof(struct zone)__.

LARGE zone is the exception, it will always contain one block and will request memory on demand:<br>
__requested size + sizeof(struct block) + sizeof(struct zone)__.

### **Allocating**

There are some macros in 'include/libdm.h' that the user can modify:
|Macro            |Explanation |
|---	             |---         |
|**MEM_TINY_MAX**    | maximum size of a TINY block. (min. is 0) |
|**MEM_SMALL_MAX**   | maximum size of a SMALL block. (min. is MEM_TINY_MAX) |
|**BLOCKS_PER_ZONE** | minimum amount of average blocks that a zone should contain|
|**PAGE_SIZE**       | the page size, by default 'getpagesize()' is used to figure it out|

TINY/SMALL example:<br>
'malloc(5)' is called 100 times, the first call will allocate a TINY zone, but only use __5 + sizeof(struct block) + sizeof(struct zone)__ bytes.<br>
For the next 99 times it does not need to request memory, because it can utilize the remaining space left in TINY zone.<br>
And each subsequent 'malloc()' call will only use __5 + sizeof(struct block)__.<br>
When a zone is filled up the next 'malloc()' call will create a new zone and repeat the process.<br>

LARGE example:<br>
When 'malloc(100000)' is called, a LARGE zone is allocated __100000 + sizeof(struct block) + sizeof(struct zone)__.
Any subsequent LARGE allocations will repeat the process.

### **Deallocating**

When a TINY or SMALL block is freed it not erased or freed, instead the block is marked as unused. Additionally if the previous or following block is free the blocks will merged into 1 free block, essentially a type of “Defragmentation”.
The zone is freed if it has no reserved blocks and there are multiple zones of the same type.

When a LARGE block is freed the entire zone is deallocated, even if there is only 1 LARGE zone.

---
## Notes

-	Library is “Thread safe”, it is safe to use is multithreaded projects.
-	'ft_memshow(int fd, int flags)' is useful debugging tool. See 'Personal help functions' section for more info.
-	'ft_malloc(size_t *size, char *hash)' same as standard malloc but takes in a 4 char hash
	and saves it in struct block->str.
	The 4 bytes where going to waste so I use them with ft_malloc() to mark individual memory blocks.
	The hash can be viewed with	'ft_memshow(1, MEM_SHOW_HASH)'.
-	'ft_memfind(char *hash)' returns a pointer to the memory with same hash. Note: Use ft_malloc().
-	'ft_mempurge(void)' frees all the memory allocated by 'malloc()', 'realloc()', 'calloc()', 'ft_malloc()'.
	Without the developer having to free every individual memory block themselves.

---

## Recreated Functions
	void	*malloc(size_t size)
		- Allocates size_t size amount of memory.
	void	*calloc(size_t num, size_t size)
		- Allocates size_t size amount of memory ans sets num amount to null.
	void	*realloc(void *ptr, size_t size)
		- Changes the memory pointed by ptr to size without loosing what it saved in it.
	void	free(void *ptr)
		- Frees the memory pointed to by *ptr.
---
## Custom Functions
	void	*ft_malloc(size_t size, char *code)
		- Allocates size_t size amount of memory and saves code for debugging.
	void	*ft_memfind(char *hash)
		- Find pointer to memory with hash.
	void	ft_mempurge(void)
		- Deletes all memory that was allocated.
	void	ft_memshow(int fd, int flags)
		- Print useful info to 'fd'.
		-----------------------------------------------------------------------
		MEM_SHOW_TINY		=	Print all TINY memory allocations.	
		MEM_SHOW_SMALL		=	Print all SMALL memory allocations.
		MEM_SHOW_LARGE		=	Print all LARGE memory allocations.
		MEM_SHOW_HEX		=	Print each allocated byte in hexadecimal.
		MEM_SHOW_CHAR		=	Print each allocated byte in hex and char. (overrides 'MEM_SHOW_HEX')
		MEM_SHOW_FREE		=	Print all memory slots that are free.
		MEM_SHOW_HASH		=	Print all hash. (Use ft_malloc)			
		-----------------------------------------------------------------------

```c
#inlude "libdm.h"

void	main(void)
{
	char *str[5];

	str[0] = malloc(10);
	str[1] = malloc(400);
	str[2] = realloc(str[2], 20);
	str[3] = ft_malloc(5, "STR");
	str[4] = malloc(2048);
	free(str[4]);
	str[4] = malloc(4096);
	ft_memshow(1, MEM_SHOW_HASH | MEM_SHOW_FREE);
	ft_mempurge();
}
```
<img src="./Images/Result1.png" alt="drawing" width="900"/>

```c
#inlude <stdio.h>
#inlude <string.h>
#inlude "libdm.h"

void	main(void)
{
	char *str;

	str = ft_malloc(sizeof(char) * 12, "TEST");
	strcpy(str, "Hello World");
	ft_memshow(1, MEM_SHOW_TINY | MEM_SHOW_HASH | MEM_SHOW_CHAR);
	ft_mempurge();
}
```
<img src="./Images/Result2.png" alt="drawing" width="900"/>

---
