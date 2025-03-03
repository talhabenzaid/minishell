/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:26:24 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/03 15:12:59 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (s1 == NULL)
	{
		return (-1);
	}
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

t_env	*ft_lstlast2(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*last;

	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast2(*lst);
	last->next = new;
}

char	*ft_strncpy(char *dest, char *src, int n)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void sigint_handler(int sig)
{
    (void)sig;
    printf("\n");

    if (g_in_readline)
	{
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void sigquit_handler(int sig)
{
    (void)sig;
    if (!g_in_readline)
        printf("Quit (core dumped)\n");
	else
		return;
}

char	*ftt_strdup2( char *s1, t_alloc **head)
{
	int		i;
	char	*d;

	i = 0;
	d = ft_malloc2((ft_strlen(s1) + 1) * sizeof(char), head);
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

char	*ftt_strdup( char *s1)
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

char	*ft_strdup2(const char *s1, t_alloc **data)
{
	size_t	i;
	char	*s2;

	i = ft_strlen(s1);
	s2 = ft_malloc2(i + 1, data);
	i = 0;
	while (s1 && s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

char	*ft_strjoin2(char const *s1, char const *s2, t_alloc **data)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup2(s2, data));
	else if (s2 == NULL)
		return (ft_strdup2(s1, data));
	str = ft_malloc2(ft_strlen(s1) + ft_strlen(s2) + 1, data);
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
