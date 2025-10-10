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

void    *ezg_add(char *name, void *target)
{
    return (ezg_alloc_handler(NO_BYTES, ADD, target, name));
}

void	ezg_free(char *name, void *ptr)
{
	ezg_alloc_handler(NO_BYTES, RELEASE, ptr, name);
}

void	ezg_clean_group(char *name)
{
	ezg_alloc_handler(NO_BYTES, CLEANUP, NO_TARGET, name);
}

void    ezg_cleanup(void)
{
    ezg_alloc_handler(NO_BYTES, CLEANALL, NO_TARGET, NO_GROUP);
}

void    ezg_clean(void)
{
    ezg_alloc_handler(NO_BYTES, CLEANALL, NO_TARGET, NO_GROUP);
}