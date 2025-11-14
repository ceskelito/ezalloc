/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_allocation_handler.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:24:50 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/13 11:25:16 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc_internal.h"
#include <stdio.h>

/* Creates a new node for the garbage collector list */
t_alloc	*new_node(void *ptr)
{
	t_alloc	*node;

	node = malloc(sizeof(t_alloc));
	if (!node)
	{
		set_error(ENOMEM, "malloc failed for node");
		return (NULL);
	}
	node->data = ptr;
	node->next = NULL;
	return (node);
}

/* Safely creates a new node and adds it to the garbage collector list */
void	*safe_new_node(t_garbage *garbage, void *ptr)
{
	t_alloc	*node;

	if (!garbage || !ptr)
		return (NULL);
	node = new_node(ptr);
	if (!node)
		return (NULL);
	if (!garbage->head)
	{
		garbage->head = node;
		garbage->tail = node;
	}
	else
	{
		garbage->tail->next = node;
		garbage->tail = node;
	}
	return (node->data);
}

/* Frees all nodes in the garbage collector list */
void	cleanup_list(t_garbage *garbage)
{
	t_alloc	*curr;
	t_alloc	*next;

	if (!garbage)
		return ;
	curr = garbage->head;
	while (curr)
	{
		next = curr->next;
		free(curr->data);
		curr->data = NULL;
		free(curr);
		curr = next;
	}
	garbage->head = NULL;
	garbage->tail = NULL;
}

/* Removes and frees a specific node from the garbage collector list */
void	release_node(t_garbage *garbage, void *target_data)
{
	t_alloc	*curr;
	t_alloc	*prev;

	if (!garbage || !target_data)
		return ;
	curr = garbage->head;
	prev = NULL;
	while (curr)
	{
		if (curr->data == target_data)
		{
			if (curr == garbage->tail)
				garbage->tail = prev;
			if (curr == garbage->head)
				garbage->head = curr->next;
			else
				prev->next = curr->next;
			free(curr->data);
			curr->data = NULL;
			free(curr);
			curr = NULL;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	*allocation_handler(size_t size, int mode, void *target,
		t_garbage *garbage)
{
	void	*new_ptr;

	new_ptr = NULL;
	if (!garbage)
		return (NULL);
	if (mode == NEW)
		return (new_node_helper(garbage, new_ptr, size));
	else if (mode == ADD)
	{
		new_ptr = safe_new_node(garbage, target);
		if (!new_ptr)
			set_error(ENOMEM, "failed to create new node");
		return (new_ptr);
	}
	else if (mode == CLEANUP)
		cleanup_list(garbage);
	else if (mode == RELEASE)
		release_node(garbage, target);
	return (NULL);
}
