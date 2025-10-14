#ifndef EZALLOC_H
# define EZALLOC_H

# include <stddef.h> // size_t definition

/* Return the last error encountered in the perror format */
char    *ez_get_error(void);

/* Allocates size bytes of memory with malloc() and saves
 * the pointer in a static linked list, before returning it.
 * Returns NULL if malloc fails. */
void	*ez_alloc(size_t size);

/* Allocates size * count bytes of memory with malloc(), fills
 * everything with 0s and saves the pointer in a static linked list,
 * before returning it. Returns NULL if malloc fails.*/
void	*ez_calloc(size_t size, size_t count);

/* Add an already allocated address at the end of the garbage list.
 * Return the pointer we passed it. */
void	*ez_add(void *data);

/* Search for the pointer in the list, and frees the pointer */
void	ez_release(void	*data);

/* Frees all the memory previously allocated by ezalloc and ezcalloc. */
void	ez_cleanup(void);

#endif
