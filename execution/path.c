/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:31:32 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 20:51:46 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**env_path(char **env, t_data *data)
{
	int		i;
	char	**path;

	path = NULL;
	if (data->default_path)
		return (ft_split(data->default_path, ':', data));
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_split(env[i] + 5, ':', data);
			break ;
		}
		i++;
	}
	return (path);
}

char	*find_path(char **path, char *cmd, t_data *data, t_alloc **head)
{
	int		i;
	char	*path1;
	char	*final_path;

	i = 0;
	if (path == NULL)
	{
		access(cmd, F_OK | X_OK);
		perror(cmd);
		free_exit_child(data, head, 127);
	}
	while (path[i])
	{
		path1 = ft_strjoin2(path[i], "/", data, head);
		final_path = ft_strjoin2(path1, cmd, data, head);
		if (access(final_path, F_OK | X_OK) == 0)
			return (final_path);
		i++;
	}
	return (NULL);
}

char	*direct_execution(char *cmd, t_data *data, t_alloc **head)
{
	if (access(cmd, F_OK) != 0)
	{
		perror(cmd);
		free_exit_child(data, head, 127);
	}
	else if (access(cmd, X_OK) != 0)
	{
		perror(cmd);
		free_exit_child(data, head, 126);
	}
	return (ft_strdup2(cmd, data, head));
}

char	*get_path(char **env, char *cmp, t_data *data, t_alloc **head)
{
	char		**path;
	char		*final_path;
	struct stat	path_stat;

	if (!cmp || !*cmp)
		return (NULL);
	if (ft_strcmp(cmp, "..") == 0)
	{
		print_error_status(cmp, " Command not found");
		free_exit_child(data, head, 127);
	}
	if (stat(cmp, &path_stat) == 0
		&& S_ISDIR(path_stat.st_mode) && ft_strchr(cmp, '/'))
	{
		print_error_status(cmp, "Is a directory");
		free_exit_child(data, head, 126);
	}
	path = env_path(env, data);
	if (cmp[0] == '.' && cmp[1] == '/')
		return (direct_execution(cmp, data, head));
	if (cmp[0] == '/')
		return (direct_execution(cmp, data, head));
	if (!data->default_path && !ft_getenv2("PATH", data))
		return (direct_execution(cmp, data, head));
	return (final_path = find_path(path, cmp, data, head), final_path);
}
