#ifndef EZGALLOC_INTERNAL_H
# define EZGALLOC_INTERNAL_H

# include <stdlib.h>

typedef enum e_mode
{
	ADD,
	CLEAN_ALL,
	CLEAN_GROUP,
	CREATE,
	NEW,
	RELEASE,
} t_mode;

/* garbage collector node */
typedef struct s_alloc
{
	void			*ptr;
	struct s_alloc	*next;
}	t_alloc;

/* garbage group node */
typedef struct s_group
{
  t_alloc         *garbage_head;
  t_alloc         *garbage_tail;
  char            *tag;
  struct s_group  *next;
} t_group;

#endif
