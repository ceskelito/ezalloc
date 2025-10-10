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

/* Global Allocation Handler

 * This function will perform all the memory's allocations and frees,
 * keeping track of every action performed. 

 * size:	Number of bytes to alloc.
 * mode:	The action to be performed.
 * target:	The pointer to free or add. NULL if is an allocation or a cleanup.
 * ext_g:	The eventually external list of allocated memory.
 * 			If NULL the handler will use his internal static variable instead.
 * 			It will be NULL only if the handler is called from the ezg_*
 * 			group of functions.
 
 * Return Value: The alloc'd memory. NULL if fails or if is a deallocation action.
*/
void	*ezalloc_handler(size_t size, int mode, void *target, t_garbage *ext_g);


#endif