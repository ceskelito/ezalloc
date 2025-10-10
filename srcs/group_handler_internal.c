#include "ezalloc.h"
#include "ezalloc_internal.h"

/* Returns a pointer to the name group */
static t_group	*get_group(t_group **head, char *name)
{
  t_group *curr;

  curr = *head;
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

    if (get_group(head, name) != NULL)
      return (NULL);
    group = malloc(sizeof(t_group));
    strcpy(group->name, name);
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

    if (mode == CLEANALL)
    {
        //action to perform
    }
    if (!name)
        return (NULL); // Add string to perror
    if (mode == CREATE)
        return (safe_new_group(&groups_head, &groups_tail, name));
    group = get_group(&groups_head, name);
    if (!group)
        return (NULL); // Add string to perror	
    return (allocation_handler(size, mode, target, group->garbage));
}

