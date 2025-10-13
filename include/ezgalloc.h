#ifndef EZGALLOC_H
# define EZGALLOC_H

# include <stddef.h>

/* Allocates size bytes of memory with malloc() and saves
** the pointer in a static linked list named ${group}, before returning it.
** Every list will be managed separatly from the others.
** Returns NULL if malloc fails. */
void	*ezg_alloc(char *group, size_t size);

/* Allocates size * count bytes of memory with malloc(), fills
** everything with 0s and saves the pointer in a static linked list,
** before returning it.
** Every "group" is a different list which can be managed separatly from the other.
** Returns NULL if malloc fails.*/
void	*ezg_calloc(char *group, size_t size, size_t count);

/* Add an already allocated address at the end of the ${group} named linked list.
 * Return the pointer we passed it. */
void	*ezg_add(char *group, void *data);

/* Search for the pointer in the "group" list, and frees the pointer*/
void    ezg_release(char *group, void *data);

/* Create a new named gargabe list;
** if a group named ${group} already exists fails.
** Return 0 on success, 1 on failure. */
int		ezg_group_create(char *group);

/* Frees all the memory previously allocated by ezg_alloc and ezg_calloc
** in the ${group} named linked list and delete the list itself. */
void	ezg_group_release(char *group);

/* Frees all the memory previously allocated by ezg_alloc and ezg_calloc. */
void	ezg_cleanup(void);

#endif
