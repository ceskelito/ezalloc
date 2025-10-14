#include "ezalloc.h"
#include "ezalloc_internal.h"
#include <errno.h>
#include <stdio.h>

static void	detach_group(t_group **head, t_group **tail, t_group *group)
{
  if (group == *head)
    *head = group->next;
  if (group == *tail)
    *tail = group->prev;
  if (group->prev)
    group->prev->next = group->next;
  if (group->next)
    group->next->prev = group->prev;
}

static void	free_group(t_group *group)
{
  if (!group)
    return ;
  free(group->name);
  free(group->garbage);
  free(group);
}

static void	delete_group(t_group **head, t_group **tail, t_group *group)
{
  if (!group)
    return ;
  detach_group(head, tail, group);
  free_group(group);
}

/* Returns a pointer to the name group */
static t_group	*get_group(t_group *head, char *name)
{
  if (!head || !name)
    return (NULL);
  for (t_group *curr = head; curr; curr = curr->next)
    if (strcmp(curr->name, name) == 0)
      return (curr);
  return (NULL);
}

static t_group	*create_group_struct(char *name)
{
  t_group	*group;

  group = calloc(1, sizeof(*group));
  if (!group)
  {
    perror("ezalloc: malloc failed for group");
    return (NULL);
  }
  group->name = strdup(name);
  if (!group->name)
  {
    perror("ezalloc: strdup failed");
    free(group);
    return (NULL);
  }
  group->garbage = calloc(1, sizeof(*group->garbage));
  if (!group->garbage)
  {
    perror("ezalloc: malloc failed for garbage structure");
    free(group->name);
    free(group);
    return (NULL);
  }
  return (group);
}

static void	append_group(t_group **head, t_group **tail, t_group *group)
{
  if (!*head)
  {
    *head = group;
    *tail = group;
    return ;
  }
  (*tail)->next = group;
  group->prev = *tail;
  *tail = group;
}

static t_group *safe_new_group(t_group **head, t_group **tail, char *name)
{
  t_group *group;

  if (get_group(*head, name))
  {
    perror("ezalloc: trying to create a group with an existent name");
    return (NULL);
  }
  group = create_group_struct(name);
  if (!group)
    return (NULL);
  append_group(head, tail, group);
  return (group);
}

static void	cleanup_all_groups(t_group **head, t_group **tail)
{
  while (*head)
  {
    t_group *next = (*head)->next;

    allocation_handler(NO_BYTES, CLEANUP, NO_DATA, (*head)->garbage);
    delete_group(head, tail, *head);
    *head = next;
  }
  *tail = NULL;
}

void	*ezg_alloc_handler(size_t size, int mode, void *target, char *name)
{
    static t_group	*groups_head;
    static t_group	*groups_tail;
  t_group		    *group;

  if (mode == CLEANUP)
  {
    cleanup_all_groups(&groups_head, &groups_tail);
    return (NULL);
  }
  if (!name)
  {
    perror("ezalloc: group name is NULL");
    return (NULL);
  }
  if (mode == CREATE_GROUP)
  {
    if (!safe_new_group(&groups_head, &groups_tail, name))
      perror("ezalloc: failed to create group");
    return (NULL);
  }
  group = get_group(groups_head, name);
  if (!group)
  {
    perror("ezalloc: group not found");
    return (NULL);
  }
  if (mode == RELEASE_GROUP || mode == DELETE_GROUP)
  {
    allocation_handler(NO_BYTES, CLEANUP, NO_DATA, group->garbage);
    if (mode == DELETE_GROUP)
      delete_group(&groups_head, &groups_tail, group);
    return (NULL);
  }
  return (allocation_handler(size, mode, target, group->garbage));
}

