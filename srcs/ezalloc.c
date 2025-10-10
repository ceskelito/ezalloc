#include "ezalloc.h"
#include "ezalloc_internal.h"

void	*ez_alloc(size_t size)
{
	return (ez_alloc_handler(size, NEW, NO_TARGET));
}

void	*ez_calloc(size_t size, size_t count)
{
	char	*new_ptr;
	size_t	i;

	new_ptr = ez_alloc_handler(size * count, NEW, NO_TARGET);
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

void	*ez_add(void	*ptr)
{
	ez_alloc_handler(NO_BYTES, ADD, ptr);
	return (ptr);
}

void	ez_free(void *ptr)
{
	ez_alloc_handler(NO_BYTES, RELEASE, ptr);
}

void	ez_cleanup(void)
{
	ez_alloc_handler(NO_BYTES, CLEAN, NO_TARGET);
}
