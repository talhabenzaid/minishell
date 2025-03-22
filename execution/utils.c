/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:26:24 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 21:40:11 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_in_readline == 1)
	{
		write(1, "^C\n", 3);
		exit_stat(130, 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (g_in_readline == 3)
	{
		g_in_readline = 4;
		close(0);
	}
}

char	*ftt_strdup(char *s1)
{
	int		i;
	char	*d;

	i = 0;
	d = malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!d)
		return (NULL);
	while (s1[i])
	{
		d[i] = s1[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

char	*ft_strdup2(char *s1, t_data *data1, t_alloc **data)
{
	size_t	i;
	char	*s2;

	i = ft_strlen(s1);
	s2 = ft_malloc2(i + 1, data1, data);
	i = 0;
	while (s1 && s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

char	*ft_strjoin2(char *s1, char*s2, t_data *data1, t_alloc **data)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup2(s2, data1, data));
	else if (s2 == NULL)
		return (ft_strdup2(s1, data1, data));
	str = ft_malloc2(ft_strlen(s1) + ft_strlen(s2) + 1, data1, data);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}
