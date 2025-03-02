/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:05:53 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/01 19:41:37 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	if (dstsize != 0)
	{
		while (i < (dstsize - 1) && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = 0;
	}
	return (src_len);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

static size_t	to_allocate(int n)
{
	unsigned int	nbr;
	size_t			size;

	nbr = n;
	if (n < 0)
	{
		size = 1;
		nbr = -n;
	}
	else if (n == 0)
		size = 1;
	else
		size = 0;
	while (nbr > 0)
	{
		size++;
		nbr = nbr / 10;
	}
	return (size);
}

char	*ft_itoa(int n, t_data *data)
{
	size_t			size;
	char			*str;
	unsigned int	nbr;

	size = to_allocate(n);
	str = (char *) ft_malloc(size + 1, data);
	str[size] = 0;
	size--;
	if (n < 0)
		nbr = -n;
	else
		nbr = n;
	while (size > 0)
	{
		str[size] = (nbr % 10 + '0');
		nbr = nbr / 10;
		size--;
	}
	if (n < 0)
		str[size] = '-';
	else
		str[size] = (nbr % 10 + '0');
	return (str);
}
