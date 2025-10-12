#include "ezalloc_internal.h"

static t_alloc	*new_node(void	*ptr)
{
	t_alloc	*node;

	node = malloc(sizeof(t_alloc));
	node->data = ptr;
	node->next = NULL;
	return (node);
}

static void	safe_new_node(t_alloc **head, t_alloc **tail, void *ptr)
{
	if (!ptr)
		return ;
	if (!*head)
	{
		*head = new_node(ptr);
		*tail = *head;
	}
	else
	{
		(*tail)->next = new_node(ptr);
		*tail = (*tail)->next;
	}
}

static void	cleanup_list(t_alloc *head)
{
	t_alloc	*tmp;

	tmp = head;
	while (tmp)
	{
		head = head->next;
		free(tmp->data);
		free(tmp);
		tmp = head;
	}
}

static void	release_node(t_alloc **head, void *target_data)
{
	t_alloc	*curr;
	t_alloc	*prev;

	if (!target_data)
		return ;
	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (curr->data == target_data)
		{
			if (curr == *head)
				*head = curr->next;
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
		safe_new_node(&garbage->head, &garbage->tail, new_ptr);
		return (new_ptr);
	}
	else if (mode == ADD)
	{
		safe_new_node(&garbage->head, &garbage->tail, target);
		return (target);
	}
	else if (mode == CLEANUP)
	{	
		cleanup_list(garbage->head);
	}
	else if (mode == RELEASE)
	{		
		release_node(&garbage->head, target);
	}
	return (NULL);
}
