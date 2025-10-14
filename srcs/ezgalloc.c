#include "ezalloc_internal.h"
#include "ezgalloc.h"

char	*ezg_get_error(void)
{
	return (get_error());
}

void    *ezg_alloc(char *name, size_t size)
{
    return(ezg_alloc_handler(size, NEW, NO_DATA, name));
}

void	*ezg_calloc(char *name, size_t size, size_t count)
{
	void	*new_ptr;

	if (count != 0 && size > SIZE_MAX / count)
	{
		errno = EOVERFLOW;
		return (NULL);
	}
	new_ptr = ezg_alloc_handler(size * count, NEW, NO_DATA, name);
	internal_bzero(new_ptr, size * count);
	return (new_ptr);
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
	if (!ezg_alloc_handler(NO_BYTES, CREATE_GROUP, NO_DATA, name))
		return (1);
	return (0);
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
	free_error();
    ezg_alloc_handler(NO_BYTES, CLEANUP, NO_DATA, NO_GROUP);
}