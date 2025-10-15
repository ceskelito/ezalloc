/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ezalloc_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:24:38 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:25:29 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EZALLOC_INTERNAL_H
# define EZALLOC_INTERNAL_H

# include <errno.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>

# define NO_BYTES 0
# define NO_DATA 0
# define NO_GROUP 0

typedef enum e_action
{
	NEW,
	ADD,
	RELEASE,
	CLEANUP,
	CREATE_GROUP,
	RELEASE_GROUP,
	DELETE_GROUP
}					t_action;

typedef enum e_err_action
{
	GET,
	SET,
	CLEAN
}					t_err_action;

/* garbage collector node */
typedef struct s_alloc
{
	void			*data;
	struct s_alloc	*next;
}					t_alloc;

/* garbage node */
typedef struct s_garbage
{
	t_alloc			*head;
	t_alloc			*tail;
}					t_garbage;

/* garbage group node */
typedef struct s_group
{
	t_garbage		*garbage;
	char			*name;
	struct s_group	*next;
	struct s_group	*prev;
}					t_group;

typedef struct s_group_context
{
	t_group			*head;
	t_group			*tail;
}					t_group_context;

/*
 * allocation_handler - Global allocation handler
 *
 * This function centralizes all memory allocations and releases,
 * keeping track of every allocated pointer for automatic cleanup.
 *
 * @size:    Number of bytes to allocate (used only in NEW mode).
 * @mode:    Operation to perform (NEW, ADD, CLEANUP, or RELEASE).
 * @target:  Pointer to add or release, depending on mode.
 *           Must be NULL for NEW and CLEANUP operations.
 * @garbage: Garbage collector context. Must not be NULL.
 *
 * Return: Pointer to the allocated memory for NEW and ADD modes,
 *         or NULL if allocation fails or for CLEANUP/RELEASE modes.
 *         Sets errno to ENOMEM on allocation failures.
 */
void	*allocation_handler(size_t size, int mode, void *target,
			t_garbage *garbage);
/*
 * ezg_alloc_handler - Groups allocation handler
 *
 * This function manages all group-based operations. It keeps track of
 * all groups created and distributes operations to be performed on them.
 * Relies on allocation_handler for operations involving memory within
 * a single group.
 *
 * @size:   Number of bytes to allocate (used only in NEW mode).
 * @mode:   Operation to perform (NEW, ADD, CLEANUP, RELEASE, CREATE_GROUP,
 *          RELEASE_GROUP, or DELETE_GROUP).
 * @target: Pointer to add or release, depending on mode.
 * @name:   Name of the group to operate on. Must not be NULL except
 *          for CLEANUP mode.
 *
 * Return: Pointer to allocated memory or created group for NEW/ADD/CREATE_GROUP,
 *         or NULL for other modes or on failure.
 *         Sets errno to ENOMEM on allocation failures, EINVAL for invalid
 *         arguments or missing groups, and EEXIST when trying to create
 *         a group that already exists. */
void	*ezg_alloc_handler(size_t size, int mode, void *target,
			char *name);

/*
 * set_error - Sets the last error message
 *
 * @str: Error message string to store
 *
 * Stores a copy of the error message for later retrieval.
 * Sets errno to ENOMEM if internal_strdup fails.
 */
void	set_error(char *str);

/*
 * get_error - Retrieves the last error message
 *
 * Return: The last error message string, or NULL if no error
 */
char	*get_error(void);

/*
 * free_error - Frees the stored error message
 *
 * Cleans up the error message storage.
 */
void	free_error(void);

/* Libraries Helper Functions*/
char	*internal_strdup(char *s);
int		internal_strcmp(char *s1, char *s2);
void	*internal_calloc(size_t nmemb, size_t size);
void	internal_bzero(void *s, size_t n);

#endif
