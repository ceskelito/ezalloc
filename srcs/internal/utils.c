/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:02 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 12:25:24 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc_internal.h"

/* Manages the last error message (set, get, or clean) */
static char	*error_handler(int action, char *str)
{
	static char	*last_error;

	if (action == SET)
	{
		free(last_error);
		last_error = NULL;
		if (!str)
			return (NULL);
		last_error = internal_strdup(str);
		if (!last_error)
			errno = ENOMEM;
	}
	else if (action == GET)
		return (last_error);
	else if (action == CLEAN)
	{
		free(last_error);
		last_error = NULL;
	}
	return (NULL);
}

inline void	set_error(char *str)
{
	error_handler(SET, str);
}

inline char	*get_error(void)
{
	return (error_handler(GET, NULL));
}

inline void	free_error(void)
{
	error_handler(CLEAN, NULL);
}
