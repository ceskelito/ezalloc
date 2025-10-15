/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:25:00 by rceschel          #+#    #+#             */
/*   Updated: 2025/10/15 17:49:42 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

unsigned int	internal_strlcat(char *dest, const char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	l;
	unsigned int	j;

	l = 0;
	while (dest[l] != '\0' && l < size)
		l++;
	j = 0;
	while (src[j] != '\0')
		j++;
	i = 0;
	if (size == 0 || l == size)
		return (l + j);
	while (i < size - l - 1 && src[i] != '\0')
	{
		dest[l + i] = src[i];
		i++;
	}
	dest[l + i] = '\0';
	return (l + j);
}

int	internal_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i++] != '\0')
		;
	return (--i);
}

size_t	internal_strlcpy(char *dest, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i])
			i++;
		return (i);
	}
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < size)
		dest[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

char	*internal_strdup(char *s)
{
	char	*str;

	str = (char *)malloc(internal_strlen(s) + 1);
	internal_strlcpy(str, (char *)s, internal_strlen(s) + 1);
	return (str);
}

int	internal_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
