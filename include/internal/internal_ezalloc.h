#ifndef INTERNAL_EZALLOC_H
# define INTERNAL_EZALLOC_H

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
	void			*ptr;
	struct s_alloc	*next;
}	t_alloc;

void	*ezalloc_handler(size_t size, int mode, void *target);


#endif