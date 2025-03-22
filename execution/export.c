/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 03:09:01 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 20:55:10 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_existing(t_env *current, char *str, char *rest, t_data *data)
{
	char	*current_rest;

	if (rest && *(rest - 1) == '+')
	{
		current_rest = ft_strchr(current->env_var, '=');
		append_env(current, rest + 1, current_rest, data);
	}
	else
	{
		free(current->env_var);
		current->env_var = ftt_strdup(str);
		if (current->env_var == NULL)
			free_exit(data);
	}
}

void	handle_new_entry(char *str, char *rest, int key_len, t_data *data)
{
	char	*new_str;

	if (rest && *(rest - 1) == '+')
	{
		new_str = ft_malloc(ft_strlen(str) + 1, data);
		ft_strncpy(new_str, str, key_len);
		ft_strncpy(new_str + key_len, rest, ft_strlen(rest) + 1);
		add_export(new_str, data);
	}
	else
		add_export(str, data);
}

void	export(char **str, t_data *data)
{
	int	i;

	i = 1;
	exit_stat(0, 1);
	while (str[i])
	{
		if (!is_valid(str[i]))
		{
			write(2, "bash: export: '", 15);
			write(2, str[i], ft_strlen(str[i]));
			write(2, "': not a valid identifier\n", 27);
			exit_stat(1, 1);
		}
		else
			check_add(str[i], data);
		i++;
	}
}

void	print_sorted_env_vars(char **env_vars)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (env_vars[i])
	{
		key = env_vars[i];
		value = ft_strchr(key, '=');
		if (value)
		{
			*value = '\0';
			value++;
			if (key[0] != '_' && key[1] != '=')
				printf("declare -x %s=\"%s\"\n", key, value);
		}
		else
			printf("declare -x %s\n", key);
		i++;
	}
}

void	export_child(char **str, t_data *data, t_alloc **head)
{
	char	**env_vars;
	int		i;
	int		j;

	j = 0;
	if (str[1] == NULL)
	{
		env_vars = sort_export(data->env, data, head);
		print_sorted_env_vars(env_vars);
	}
	else
	{
		i = 1;
		while (str[i])
		{
			if (!is_valid(str[i]))
			{
				write(2, "bash: export: '", 15);
				print_error_status(str[i], "': not a valid identifier\n");
				j = 1;
			}
			i++;
		}
	}
	free_exit_child(data, head, j);
}
