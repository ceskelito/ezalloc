#include "ezalloc.h"
#include "ezalloc_internal.h"

void	*ez_alloc_handler(size_t size, int mode, void *target)
{
	return (allocation_handler(size, mode, target, NULL));
}

void	*ezg_alloc_handler(size_t size, int mode, void *target, t_garbage *ext_g)
{
	if (!ext_g)
		return (NULL); // Add string to perror
	return (allocation_handler(size, mode, target, ext_g));
}
