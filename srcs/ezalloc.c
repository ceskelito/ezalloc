/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ezalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:07 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:25:23 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc.h"
#include "ezalloc_internal.h"

/* Wrapper for allocation_handler with static garbage collector */
static void	*ez_alloc_handler(size_t size, int mode, void *target)
{
	static t_garbage	garbage;

	return (allocation_handler(size, mode, target, &garbage));
}

char	*ez_get_error(void)
{
	return (get_error());
}

void	*ez_alloc(size_t size)
{
	return (ez_alloc_handler(size, NEW, NO_DATA));
}

void	*ez_calloc(size_t size, size_t count)
{
	void	*new_ptr;

	if (count != 0 && size > SIZE_MAX / count)
	{
		errno = EOVERFLOW;
		return (NULL);
	}
	new_ptr = ez_alloc_handler(size * count, NEW, NO_DATA);
	internal_bzero(new_ptr, size * count);
	return (new_ptr);
}

void	*ez_add(void *data)
{
	return (ez_alloc_handler(NO_BYTES, ADD, data));
}

void	ez_release(void *data)
{
	ez_alloc_handler(NO_BYTES, RELEASE, data);
}

void	ez_cleanup(void)
{
	free_error();
	ez_alloc_handler(NO_BYTES, CLEANUP, NO_DATA);
}
