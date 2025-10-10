#ifndef EZALLOC_INTERNAL_H
# define EZALLOC_INTERNAL_H

# include <stdlib.h>

typedef enum e_mode
{
	NEW,
	CLEAN,
	RELEASE,
	ADD
} t_mode;

/* garbage collector node */
typedef struct s_alloc
{
	void			*data;
	struct s_alloc	*next;
}	t_alloc;

/* garbage group node */
typedef struct s_garbage
{
  t_alloc         *head;
  t_alloc         *tail;
} t_garbage;

/*
 * ezalloc_handler - Global allocation handler
 *
 * This function centralizes all memory allocations and releases,
 * keeping track of every allocated pointer for automatic cleanup.
 *
 * @size:   Number of bytes to allocate (used only in NEW mode).
 * @mode:   Operation to perform (e.g. NEW, ADD, CLEAN, RELEASE).
 * @target: Pointer to add or release, depending on mode.
 *          Must be NULL for allocation or cleanup operations.
 * @ext_g:  Optional external garbage collector context.
 *          If NULL, the internal static context will be used instead.
 *          It should be non-NULL only when called from ezg_* family of functions
 * 			(which maintain their own, grouped, allocation lists).
 *
 * Return:  Pointer to the allocated memory, or NULL if allocation fails
 *          or if the operation does not produce a new allocation.
 */
void	*ezalloc_handler(size_t size, int mode, void *target, t_garbage *ext_g);


#endif