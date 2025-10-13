#include "ezalloc_internal.h"
#include "ezgalloc.h"

void    *ezg_alloc(char *name, size_t size)
{
    return(ezg_alloc_handler(size, NEW, NO_DATA, name));
}

void	*ezg_calloc(char *name, size_t size, size_t count)
{
	char	*new_ptr;
    size_t  i;

	new_ptr = ezg_alloc_handler(size * count, NEW, NO_DATA, name);
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < size * count)
	{
		new_ptr[i] = 0;
		++i;
	}
	return ((void *)new_ptr);
}

void    *ezg_add(char *name, void *data)
{
    return (ezg_alloc_handler(NO_BYTES, ADD, data, name));
}

void	ezg_release(char *name, void *data)
{
	ezg_alloc_handler(NO_BYTES, RELEASE, data, name);
}

int 	ezg_group_create(char *name)
{
	ezg_alloc_handler(NO_BYTES, CREATE_GROUP, NO_DATA, name);
	return 0;
}

void	ezg_group_release(char *name)
{
	ezg_alloc_handler(NO_BYTES, RELEASE_GROUP, NO_DATA, name);
}

void	ezg_group_delete(char *name)
{
	ezg_alloc_handler(NO_BYTES, DELETE_GROUP, NO_DATA, name);
}

void    ezg_cleanup(void)
{
    ezg_alloc_handler(NO_BYTES, CLEANUP, NO_DATA, NO_GROUP);
}