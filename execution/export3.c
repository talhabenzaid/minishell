/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:08:50 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 19:35:17 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv2(char *str, t_data *data)
{
	t_env	*current;
	char	*value;

	if (ft_strcmp(str, "PATH") == 0 && data->default_path)
		return (ft_strdup(data->default_path, data));
	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, str) == 0)
		{
			value = ft_strchr(current->env_var, '=');
			if (value == NULL)
				return (ft_strdup("", data));
			else
				return (ft_strdup(value + 1, data));
		}
		current = current->next;
	}
	return (NULL);
}

static char	*extract_key(char *str, int key_len, t_data *data)
{
	char	*key;

	key = ft_malloc(key_len + 1, data);
	ft_strncpy(key, str, key_len);
	key[key_len] = '\0';
	return (key);
}

static int	get_key_len(char *str, char *rest, int *append)
{
	if (rest && *(rest - 1) == '+')
	{
		*append = 1;
		return ((rest - 1) - str);
	}
	if (rest)
		return (rest - str);
	return (ft_strlen(str));
}

static void	process_env(char *str, t_data *data, char *key, int key_len)
{
	t_env	*current;

	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, key) == 0)
		{
			update_existing(current, str, ft_strchr(str, '='), data);
			return ;
		}
		current = current->next;
	}
	handle_new_entry(str, ft_strchr(str, '='), key_len, data);
}

void	check_add(char *str, t_data *data)
{
	char	*rest;
	int		key_len;
	int		append;
	char	*key;

	if (ft_strcmp("PATH", str) == 0 && data->default_path)
	{
		str = ft_strjoin(str, "=", data);
		str = ft_strjoin(str, data->default_path, data);
		free(data->default_path);
		data->default_path = NULL;
	}
	rest = ft_strchr(str, '=');
	if (!rest && ft_getenv2(str, data))
		return ;
	append = 0;
	key_len = get_key_len(str, rest, &append);
	key = extract_key(str, key_len, data);
	process_env(str, data, key, key_len);
}
