#include "ezalloc.h"
#include "ezalloc_internal.h"
#include <errno.h>
#include <stdio.h>

static void		delete_group(t_group **head, t_group **tail, t_group *group)
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

/* Returns a pointer to the name group */
static t_group	*get_group(t_group *head, char *name)
{
  t_group *curr;

  if (!head || !name)
    return (NULL);
  curr = head;
  while(curr)
  {
    if (strcmp(curr->name, name) == 0)
      return (curr);
    curr = curr->next;
  }
  return (NULL);
}

static t_group *safe_new_group(t_group **head, t_group **tail, char *name)
{
    t_group *group;

    if (get_group(*head, name))
    {
    	perror("ezalloc: trying to create a group with an existent name");
		return (NULL);
	}
    group = malloc(sizeof(t_group));
    if (!group)
		return (NULL);
    group->name = strdup(name);
    if (!group->name)
    {
        free(group);
        perror("ezalloc: strdup failed");
        return (NULL);
    }
    group->garbage = malloc(sizeof(t_garbage));
    if (!group->garbage)
    {
        free(group->name);
        free(group);
        perror("ezalloc: malloc failed for garbage structure");
        return (NULL);
    }
    group->garbage->head = NULL;
    group->garbage->tail = NULL;
    group->next = NULL;
    group->prev = NULL;
    if (!*head)
    {
		*head = group;
		*tail = *head;
    }
    else
	{
		(*tail)->next = group;
		group->prev = *tail;
		*tail = (*tail)->next;
	}
	return (group);
}

void	*ezg_alloc_handler(size_t size, int mode, void *target, char *name)
{
    static t_group	*groups_head;
    static t_group	*groups_tail;
    t_group			    *group ;

	if (mode == CLEANUP)
	{
	    while (groups_head)
	    {
	        group = groups_head->next;
	        ezg_alloc_handler(NO_BYTES, DELETE_GROUP, NO_DATA, groups_head->name);
	        groups_head = group;
	    }
	    groups_tail = NULL;
	    return (NULL);
	}
	else if (!name)
    {
      perror("ezalloc: group name is NULL");
      return (NULL);
    }
    if (mode == CREATE_GROUP)
    {
		if (!safe_new_group(&groups_head, &groups_tail, name))
		{
		    perror("ezalloc: failed to create group");
		}
		return (NULL);
    }
    else if (!(group = get_group(groups_head, name)))
    {
        perror("ezalloc: group not found");
        return (NULL);
    }
    if (mode == RELEASE_GROUP)
    {
    	allocation_handler(size, CLEANUP, NO_DATA, group->garbage);
    }
    else if (mode == DELETE_GROUP)
    {
        allocation_handler(size, CLEANUP, NO_DATA, group->garbage);
        delete_group(&groups_head, &groups_tail, group);
    }
    else
    {
		return (allocation_handler(size, mode, target, group->garbage));
    }
    return (NULL);
}

