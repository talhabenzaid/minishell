/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:28:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/01 20:59:25 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (c);
	return (0);
}

char	*ft_getenv(char *str, t_data *data)
{
	t_env	*current;

	if (data->env == NULL && strcmp(str, "PATH") == 0)
		return (ft_strdup(data->default_path, data));
	current = data->env;
	while (current)
	{
		if (ft_strncmp(current->env_var, str, ft_strlen(str)) == 0)
			return (ft_strdup(ft_strchr(current->env_var, '=') + 1, data));
		current = current->next;
	}
	return (ft_strdup("", data));
}

char	*get_expanded_value(char *token, int i, int *j, t_data *data)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
	{
		(*j)++;
		return (ft_itoa(data->exit_status, data));
	}
	if (!ft_isalnum(token[i]) && token[i] && token[i + *j] != '_')
	{
		while (token[i + *j] && (token[i + *j] != '$'))
			(*j)++;
	}
	else
	{
		while (token[i + *j] && (token[i + *j] != '$'))
		{
			if (!ft_isalnum(token[i + *j]) && token[i + *j] != '_')
				break ;
			(*j)++;
		}
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), data);
	ft_strlcpy(expanded, token + i, *j + 1);
	return (ft_getenv(expanded, data));
}

char	*handle_multiple_dollars(char *token, int count, int i, t_data *data)
{
	char	*previous;
	char	*expanded;
	int		k;
	char	*next;

	if (count % 2 != 0)
		count--;
	expanded = ft_strdup("", data);
	previous = ft_malloc(sizeof(char) * i + 1, data);
	ft_strlcpy(previous, token, i + 1);
	k = 0;
	while (token[i + count + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + i + count, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}

char	*build_exp_str(char *token, char *previous, int i, t_data *data)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, i, &j, data);
	previous = ft_malloc(sizeof(char) * i, data);
	ft_strlcpy(previous, token, i);
	while (token[i + j + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + i + j, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}
