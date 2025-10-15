/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ezgalloc_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:10 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:25:22 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc_internal.h"
#include "ezgalloc.h"

char	*ezg_get_error(void)
{
	return (get_error());
}

int	ezg_group_create(char *name)
{
	if (!ezg_alloc_handler(NO_BYTES, CREATE_GROUP, NO_DATA, name))
		return (1);
	return (0);
}

void	ezg_group_release(char *name)
{
	ezg_alloc_handler(NO_BYTES, RELEASE_GROUP, NO_DATA, name);
}

void	ezg_group_delete(char *name)
{
	ezg_alloc_handler(NO_BYTES, DELETE_GROUP, NO_DATA, name);
}
