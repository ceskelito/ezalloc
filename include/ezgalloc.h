/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ezgalloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:24:46 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:46:01 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EZGALLOC_H
# define EZGALLOC_H

# include <stddef.h>

/*
 * ezg_get_error - Retrieves the last error message
 *
 * Return: A string describing the last error that occurred,
 * or NULL if no error. */
char	*ezg_get_error(void);

/*
 * ezg_alloc - Allocates and tracks memory in a named group
 *
 * @group: Name of the group to allocate in
 * @size:  Number of bytes to allocate
 *
 * Allocates size bytes of memory and saves the pointer in a named
 * linked list (group) for separate management.
 *
 * Return: Pointer to the allocated memory, or NULL if allocation fails.
 *         On failure, sets errno to ENOMEM or EINVAL if group not found. */
void	*ezg_alloc(char *group, size_t size);

/*
 * ezg_calloc - Allocates and zeros memory in a named group
 *
 * @group: Name of the group to allocate in
 * @size:  Size of each element
 * @count: Number of elements
 *
 * Allocates size * count bytes of memory, zeros it, and saves the pointer
 * in a named linked list (group) for separate management.
 *
 * Return: Pointer to the zeroed memory, or NULL if allocation fails or
 *         if size * count would overflow. On overflow, sets errno to EOVERFLOW.
 *         On allocation failure, sets errno to ENOMEM or EINVAL
 *         if group not found. */
void	*ezg_calloc(char *group, size_t size, size_t count);

/*
 * ezg_add - Adds an existing pointer to a named group
 *
 * @group: Name of the group to add the pointer to
 * @data:  Pointer to memory previously allocated with malloc/calloc
 *
 * Adds an already allocated address to the named group's garbage collector list.
 *
 * Return: The pointer passed as argument, or NULL if the operation fails.
 *         On failure, sets errno to ENOMEM or EINVAL if group not found. */
void	*ezg_add(char *group, void *data);

/*
 * ezg_release - Frees a specific tracked pointer in a group
 *
 * @group: Name of the group
 * @data:  Pointer to free
 *
 * Searches for the pointer in the group's tracked list
 * and frees it immediately. */
void	ezg_release(char *group, void *data);

/*
 * ezg_group_create - Creates a new named group
 *
 * @group: Name for the new group
 *
 * Creates a new named garbage collector group. If a group with the same
 * name already exists, the operation fails.
 *
 * Return: 0 on success, 1 on failure. On failure, sets errno to EEXIST
 *         if group already exists, or ENOMEM if allocation fails. */
int		ezg_group_create(char *group);

/*
 * ezg_group_release - Frees all memory in a group
 *
 * @group: Name of the group to release
 *
 * Frees all tracked memory in the specified group but keeps the group
 * structure alive for future allocations. */
void	ezg_group_release(char *group);

/*
 * ezg_group_delete - Deletes a group and frees its memory
 *
 * @group: Name of the group to delete
 *
 * Frees all tracked memory in the specified group
 * and removes the group itself. */
void	ezg_group_delete(char *group);

/*
 * ezg_cleanup - Frees all groups and their memory
 *
 * Deletes all groups and frees all tracked memory across all groups.
 * Cleans up error messages. */
void	ezg_cleanup(void);

#endif
