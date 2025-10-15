/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:02 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 13:10:30 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezalloc_internal.h"

static char	*strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (ft_calloc(1, sizeof(char)));
	if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = ft_calloc(len1 + len2 + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcat(str, s2, len1 + len2 + 1);
	return (str);
}

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
