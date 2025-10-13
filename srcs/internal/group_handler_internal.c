#include "ezalloc.h"
#include "ezalloc_internal.h"

static void     cleanup_groups_list(t_group **head, t_group **tail)
{
    t_group *curr;
    t_group *prev;

    if (!*head || !*tail)
      return;
    curr = *head;
    prev = NULL;
    while (curr)
    {
        allocation_handler(NO_BYTES, CLEANUP, NO_DATA, curr->garbage);
        prev = curr;
		    curr = curr->next;
        free(prev->name);
        free(prev->garbage);
        free(prev);
    }
}

static void     delete_group(t_group **head, t_group **tail, char *target_name)
{
    t_group *curr;
    t_group *prev;

    if (!*head || !*tail)
      return ;
    curr = *head;
    prev = NULL;

    while (curr)
    {
      if (strcmp(curr->name, target_name) == 0)
      {
          if (curr == *tail)
            *tail = prev;
          if (curr == *head)
            *head = curr->next;
          else
            prev->next = curr->next;
          free(curr->name);
          free(curr->garbage);
          free(curr);
          return;
      }
    prev = curr;
		curr = curr->next;
    }
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
      return (NULL);
    group = malloc(sizeof(t_group));
    if (!group)
      return (NULL);
    group->name = strdup(name);
    group->garbage = NULL;
    group->next = NULL;
    if (!*head)
    {
		  *head = group;
		  *tail = *head;
    }
	  else
	  {
	  	(*tail)->next = group;
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
        cleanup_groups_list(&groups_head, &groups_tail);
        return (NULL);
    } else if (!name)
    {
      //set error
      return (NULL);
    }
    if (mode == CREATE_GROUP)
    {
        //if (group)
          // set error (group exist)
        //else  
          safe_new_group(&groups_head, &groups_tail, name);
        return (NULL);
    }
    else if (!(group = get_group(groups_head, name)))
    {
        //set error
        return (NULL);
    }
    if (mode == RELEASE_GROUP)
    {
        allocation_handler(size, CLEANUP, NO_DATA, group);
    }
    else if (mode == DELETE_GROUP)
    {
        allocation_handler(size, CLEANUP, NO_DATA, group);
        delete_group(name);
    }
    else
    {
      return (allocation_handler(size, mode, target, group));
    }
}

