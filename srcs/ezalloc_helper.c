/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ezalloc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:07 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:25:23 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc.h"
#include "ezalloc_internal.h"

/* Wrapper for allocation_handler with static garbage collector */
void	*ez_alloc_handler(size_t size, int mode, void *target)
{
	static t_garbage	garbage;

	return (allocation_handler(size, mode, target, &garbage));
}

char	*ez_get_error(void)
{
	return (get_error());
}
