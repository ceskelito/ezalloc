#ifndef EZALLOC_INTERNAL_H
# define EZALLOC_INTERNAL_H

# include <stdlib.h>
# include <string.h>	

# define NO_BYTES	0
# define NO_DATA	NULL
# define NO_GROUP	NULL

typedef enum e_action
{
	NEW,
	ADD,
	RELEASE,
	CLEANUP,
	CREATE_GROUP,	// only for groups
	RELEASE_GROUP,	// only for groups
	DELETE_GROUP	// only for groups
} t_action;


/* garbage collector node */
typedef struct s_alloc
{
	void			*data;
	struct s_alloc	*next;
}	t_alloc;

/* garbage node */
typedef struct s_garbage
{
	t_alloc         *head;
	t_alloc         *tail;
} t_garbage;

/* garbage group node */
typedef struct s_group
{
	t_garbage		*garbage;
	char            *name;
	struct s_group  *next;
	struct s_group	*prev;
} t_group;

typedef struct s_group_context
{
	t_group	*head;
	t_group	*tail;
} t_group_context;

/*
 * allocation_handler - Global allocation handler
 *
 * This function centralizes all memory allocations and releases,
 * keeping track of every allocated pointer for automatic cleanup.
 *
 * @size:    Number of bytes to allocate (used only in NEW mode).
 * @mode:    Operation to perform (e.g. NEW, ADD, CLEANUP, RELEASE).
 * @target:  Pointer to add or release, depending on mode.
 *           Must be NULL for allocation or cleanup operations.
 * @garbage: Garbage collector context. Can't be NULL.
 *
 * Return:  Pointer to the allocated memory, or NULL if allocation fails
 *          or if the operation does not produce a new allocation.
 */
void	*allocation_handler(size_t size, int mode, void *target, t_garbage *garbage);

void	*ezg_alloc_handler(size_t size, int mode, void *target, char *name);
#endif
