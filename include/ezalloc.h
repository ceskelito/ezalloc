#ifndef EZALLOC_H
# define EZALLOC_H

# include <stddef.h> // size_t definition

/*
 * ez_get_error - Retrieves the last error message
 *
 * Return: A string describing the last error that occurred, or NULL if no error
 */
char    *ez_get_error(void);

/*
 * ez_alloc - Allocates and tracks memory
 *
 * @size: Number of bytes to allocate
 *
 * Allocates size bytes of memory with malloc() and saves the pointer
 * in a static linked list for automatic cleanup later.
 *
 * Return: Pointer to the allocated memory, or NULL if allocation fails.
 *         On failure, sets errno to ENOMEM.
 */
void	*ez_alloc(size_t size);

/*
 * ez_calloc - Allocates and zeros memory
 *
 * @size:  Size of each element
 * @count: Number of elements
 *
 * Allocates size * count bytes of memory with malloc(), zeros the memory,
 * and saves the pointer in a static linked list for automatic cleanup later.
 *
 * Return: Pointer to the zeroed memory, or NULL if allocation fails or
 *         if size * count would overflow. On overflow, sets errno to EOVERFLOW.
 *         On allocation failure, sets errno to ENOMEM.
 */
void	*ez_calloc(size_t size, size_t count);

/*
 * ez_add - Adds an existing pointer to the garbage collector
 *
 * @data: Pointer to memory previously allocated with malloc/calloc
 *
 * Adds an already allocated address to the garbage collector list,
 * so it will be freed when ez_cleanup() is called.
 *
 * Return: The pointer passed as argument, or NULL if the operation fails.
 *         On failure, sets errno to ENOMEM.
 */
void	*ez_add(void *data);

/*
 * ez_release - Frees a specific tracked pointer
 *
 * @data: Pointer to free
 *
 * Searches for the pointer in the tracked list and frees it immediately,
 * removing it from the garbage collector.
 */
void	ez_release(void	*data);

/*
 * ez_cleanup - Frees all tracked memory
 *
 * Frees all memory previously allocated by ez_alloc() and ez_calloc(),
 * and all pointers added with ez_add(). Cleans up error messages.
 */
void	ez_cleanup(void);

#endif
