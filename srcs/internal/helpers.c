/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:49:00 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 18:38:48 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc.h"
#include "ezalloc_internal.h"

t_group	*create_group_helper(t_group_context *groups, char *name)
{
	t_group	*group;

	group = get_group(groups->head, name);
	if (group)
	{
		set_error(EEXIST, "group already exists");
		return (NULL);
	}
	group = safe_new_group(&groups->head, &groups->tail, name);
	if (!group)
	{
		set_error(ENOMEM, "failed to create group");
		return (NULL);
	}
	return (group);
}

void	delete_all_groups(t_group_context *groups)
{
	while (groups->head)
		ezg_alloc_handler(NO_BYTES, DELETE_GROUP, NO_DATA, groups->head->name);
	groups->tail = NULL;
}

void	*new_node_helper(t_garbage *garbage, void *new_ptr, size_t size)
{
	new_ptr = malloc(size);
	if (!new_ptr)
	{
		set_error(ENOMEM, "malloc failed");
		return (NULL);
	}
	if (!safe_new_node(garbage, new_ptr))
	{
		set_error(ENOMEM, "failed to create new node");
		free(new_ptr);
		new_ptr = NULL;
	}
	return (new_ptr);
}
