#include "ezalloc_internal.h"

static t_alloc	*new_node(void	*ptr)
{
	t_alloc	*node;

	node = malloc(sizeof(t_alloc));
	node->data = ptr;
	node->next = NULL;
	return (node);
}

static t_alloc *safe_new_node(t_alloc **head, t_alloc **tail, void *ptr)
{
	if (!ptr)
		return (NULL);
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
	return (ptr);

}

static void	clean_garbage_list(t_alloc *head)
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

static void	clean_garbage_node(t_alloc **head, void *target_to_free)
{
	t_alloc	*curr;
	t_alloc	*prev;

	if (target_to_free == NULL)
		return ;
	curr = *head;
	prev = NULL;
	while (curr != NULL)
	{
		if (curr->data == target_to_free)
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

void	*ezalloc_handler(size_t size, int mode, void *target, t_garbage *ext_g)
{
	static t_garbage	garbage;
	t_garbage			*g;
	void				*new_ptr;

	if (ext_g)
		g = ext_g;
	else
		g = &garbage;
	if (mode == NEW)
	{
		new_ptr = malloc(size);
		if (!new_ptr)
			return (NULL);
		safe_new_node(&g->head, &g->tail, new_ptr);
		return (new_ptr);
	}
	else if (mode == ADD)
	{
		if (!target)
			return (NULL);
		safe_new_node(&g->head, &g->tail, target);
		return (target);
	}
	else if (mode == CLEAN)
		clean_garbage_list(g->head);
	else if (mode == RELEASE)
		clean_garbage_node(&g->head, target);
	return (NULL);
}