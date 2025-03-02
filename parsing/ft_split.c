/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:16:39 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/24 20:50:40 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_seperator(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

size_t	word_count(char const *s, char sep)
{
	size_t	i;
	size_t	wc;

	i = 0;
	wc = 0;
	while (s[i])
	{
		while (s[i] && is_seperator(s[i], sep))
			i++;
		if (s[i] && !is_seperator(s[i], sep))
		{
			wc++;
			while (!is_seperator(s[i], sep) && s[i])
				i++;
		}
	}
	return (wc);
}

static char	*ft_allocate(char const *s, char sep, t_data *data)
{
	size_t	i;
	char	*ptr;
	size_t	len;

	len = 0;
	while (s[len] && !is_seperator(s[len], sep))
		len++;
	ptr = ft_malloc(len + 1, data);
	i = 0;
	while (i < len)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

char	**ft_split(char const *s, char sep, t_data *data)
{
	char	**arr;
	size_t	i;

	if (s == NULL)
		return (NULL);
	arr = ft_malloc(sizeof(char *) * (word_count(s, sep) + 1), data);
	i = 0;
	while (*s)
	{
		while (*s && is_seperator(*s, sep))
			s++;
		if (*s && !is_seperator(*s, sep))
		{
			arr[i] = ft_allocate(s, sep, data);
			i++;
			while (*s && !is_seperator(*s, sep))
				s++;
		}
	}
	arr[i] = NULL;
	return (arr);
}
