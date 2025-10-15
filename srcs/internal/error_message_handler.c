/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:02 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 17:23:21 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc_internal.h"

static char	*strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (internal_calloc(1, sizeof(char)));
	if (!s1)
		return (internal_strdup(s2));
	else if (!s2)
		return (internal_strdup(s1));
	len1 = internal_strlen(s1);
	len2 = internal_strlen(s2);
	str = internal_calloc(len1 + len2 + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	internal_strlcpy(str, s1, len1 + 1);
	internal_strlcat(str, s2, len1 + len2 + 1);
	return (str);
}

/* Manages the last error message (set, get, or clean) */
static char	*error_handler(int action, int errnum, char *str)
{
	static char	*last_error;

	if (action == SET)
	{
		errno = errnum;
		free(last_error);
		last_error = NULL;
		if (!str)
			return (NULL);
		last_error = strjoin("ezalloc: ", str);
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

inline void	set_error(int errnum, char *str)
{
	error_handler(SET, errnum, str);
}

inline char	*get_error(void)
{
	return (error_handler(GET, 0, NULL));
}

inline void	free_error(void)
{
	error_handler(CLEAN, 0, NULL);
}
