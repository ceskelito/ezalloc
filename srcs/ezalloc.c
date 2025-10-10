#include "ezalloc.h"
#include "ezalloc_internal.h"

void	*ez_alloc(size_t size)
{
	return (ezalloc_handler(size, NEW, NULL, NULL));
}

void	*ez_calloc(size_t size, size_t count)
{
	char	*new_ptr;
	size_t	i;

	new_ptr = ezalloc_handler(size * count, NEW, NULL, NULL);
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
	ezalloc_handler(0, ADD, ptr, NULL);
	return (ptr);
}

void	ez_free(void *ptr)
{
	ezalloc_handler(0, RELEASE, ptr, NULL);
}

void	ez_cleanup(void)
{
	ezalloc_handler(0, CLEAN, NULL, NULL);
}