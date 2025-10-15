/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:24:53 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 13:28:25 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc.h"
#include "ezalloc_internal.h"
#include <stdio.h>

/* Removes a group from the linked list and frees its resources */
static void	delete_group(t_group **head, t_group **tail, t_group *group)
{
	if (!group)
		return ;
	if (group == *head)
		*head = group->next;
	if (group == *tail)
		*tail = group->prev;
	if (group->prev)
		group->prev->next = group->next;
	if (group->next)
		group->next->prev = group->prev;
	free(group->name);
	free(group->garbage);
	free(group);
}

/* Searches for a group by name in the linked list.
 * Returns a pointer to the name group */
static t_group	*get_group(t_group *head, char *name)
{
	t_group	*curr;

	if (!head || !name)
		return (NULL);
	curr = head;
	while (curr)
	{
		if (internal_strcmp(curr->name, name) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/* Creates a new group and adds it to the linked list */
static t_group	*safe_new_group(t_group **head, t_group **tail, char *name)
{
	t_group	*group;

	if (get_group(*head, name))
		return (NULL);
	group = internal_calloc(1, sizeof(t_group));
	if (!group)
		return (errno = ENOMEM, NULL);
	group->name = internal_strdup(name);
	if (!group->name)
		return (free(group),
			errno = ENOMEM, NULL);
	group->garbage = internal_calloc(1, sizeof(t_garbage));
	if (!group->garbage)
		return (free(group->name),
			free(group),
			errno = ENOMEM, NULL);
	if (!*head)
		*head = group;
	else
	{
		(*tail)->next = group;
		group->prev = *tail;
	}
	*tail = group;
	return (group);
}

void	delete_all_groups(t_group_context *groups)
{
	while (groups->head)
		ezg_alloc_handler(NO_BYTES, DELETE_GROUP, NO_DATA, groups->head->name);
	groups->tail = NULL;
}

/* You can find an exhaustive description in ezalloc_internal.h*/
void	*ezg_alloc_handler(size_t size, int mode, void *target, char *name)
{
	static t_group_context	groups;
	t_group					*group;

	if (mode == CLEANUP)
		return (delete_all_groups(&groups), NULL);
	if (!name)
		return (errno = EINVAL, set_error("group name is NULL"), NULL);
	group = get_group(groups.head, name);
	if (mode == CREATE_GROUP)
	{
		if (group)
			return (errno = EEXIST, set_error("group already exists"), NULL);
		group = safe_new_group(&groups.head, &groups.tail, name)
		if (!group)
			return (errno = ENOMEM, set_error("failed to create group"), NULL);
		return (group);
	}
	if (!group)
		return (errno = EINVAL, set_error("group not found"), NULL);
	if (mode == RELEASE_GROUP || mode == DELETE_GROUP)
	{
		allocation_handler(size, CLEANUP, NO_DATA, group->garbage);
		if (mode == DELETE_GROUP)
			delete_group(&groups.head, &groups.tail, group);
		return (NULL);
	}
	return (allocation_handler(size, mode, target, group->garbage));
}
