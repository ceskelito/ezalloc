#include "ezalloc_internal.h"
#include "ezgalloc.h"

void    *ezg_alloc(char *name, size_t size)
{
    return(ezg_alloc_handler(size, NEW, NO_TARGET, name));
}

void	*ezg_calloc(char *name, size_t size, size_t count)
{
	char	*new_ptr;
    size_t  i;

	new_ptr = ezg_alloc_handler(size * count, NEW, NO_TARGET, name);
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

void	ezg_release_group(char *name)
{
	ezg_alloc_handler(NO_BYTES, RELEASE_GROUP, NO_TARGET, name);
}

void    ezg_cleanup(void)
{
    ezg_alloc_handler(NO_BYTES, CLEANUP, NO_TARGET, NO_GROUP);
}