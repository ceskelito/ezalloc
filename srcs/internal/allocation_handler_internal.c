#include "ezalloc_internal.h"
#include <errno.h>
#include <stdio.h>

static t_alloc	*new_node(void	*ptr)
{
	t_alloc	*node;

	node = malloc(sizeof(t_alloc));
	if (!node)
	{
		perror("ezalloc: malloc failed for node");
		return (NULL);
	}
	node->data = ptr;
	node->next = NULL;
	return (node);
}

static void	*safe_new_node(t_garbage *garbage, void *ptr)
{
	t_alloc *node;

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

static void	cleanup_list(t_garbage *garbage)
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
        free(curr);
        curr = next;
    }
    garbage->head = NULL;
    garbage->tail = NULL;
}

static void	release_node(t_garbage *garbage, void *target_data)
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
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	*allocation_handler(size_t size, int mode, void *target, t_garbage *garbage)
{
	void	*new_ptr;

	if (!garbage)
		return (NULL);
	if (mode == NEW)
	{
		new_ptr = malloc(size);
		if (!new_ptr)
		{
			perror("ezalloc: malloc failed")
			return (, NULL);
		}
		//return (safe_new_node(garbage, new_ptr)) ? (new_ptr) : (free(new_ptr), NULL);
		if (!safe_new_node(garbage, new_ptr))
		{
			perror("ezalloc: failed to create new node");
    		free(new_ptr);
 			return NULL;
		}
		return (new_ptr);
	}
	else if (mode == ADD)
	{
		new_ptr = safe_new_node(garbage, target);
		return (new_ptr) ? (new_ptr) : (perror("ezalloc: failed to create new node"), NULL);
	}
	else if (mode == CLEANUP)
	{	
		cleanup_list(garbage);
	}
	else if (mode == RELEASE)
	{		
		release_node(garbage, target);
	}
	return (NULL);
}
