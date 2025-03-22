/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:38:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 19:40:40 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	addenv(char **env, t_data *data)
{
	int		i;

	i = 0;
	while (env[i])
	{
		check_add(env[i], data);
		i++;
	}
	shlvl_init(data);
}

void	empty_env_init(t_data *data)
{
	char	cwd[PATH_MAX];
	char	*path;

	path = getcwd(cwd, PATH_MAX);
	if (!path)
		perror("shell-init: error retrieving current directory:"
			"getcwd: cannot access parent directories");
	else
		check_add(ft_strjoin("PWD=", path, data), data);
	shlvl_init(data);
	check_add("_=./minishell", data);
}

int	is_inenv(char *str, t_data *data)
{
	t_env	*current;

	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, str) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	env_init(char **env, t_data *data)
{
	if (env == NULL || *env == NULL)
		empty_env_init(data);
	else
		addenv(env, data);
	if (is_inenv("PATH", data) == 0)
	{
		data->default_path = ftt_strdup(PATH);
		if (data->default_path == NULL)
		{
			exit_stat(1, 1);
			free_exit(data);
		}
	}
	check_add("OLDPWD", data);
}

void	data_init(t_data *data, int argc, char **argv)
{
	(void) argc;
	(void) argv;
	exit_stat(0, 1);
	data->commands = NULL;
	data->pwd = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->alloc = NULL;
	data->default_path = NULL;
	data->env = NULL;
	data->fds = NULL;
	data->fd_count = 0;
	data->token_size = 0;
	data->command_count = 0;
	data->fd_write = -1;
}
