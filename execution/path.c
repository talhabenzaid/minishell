/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:31:32 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/02 17:56:03 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**env_path(char **env, t_data *data)
{
	int		i;
	char	**path;

	path = NULL;
	if (env == NULL || *env == NULL)
        return(ft_split(data->default_path, ':', data));	
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
	(void) data;
	if (path == NULL)
	{
		access(cmd, F_OK | X_OK);
		perror(cmd);
		exit(127);
	}
	while (path[i])
	{
		path1 = ft_strjoin2(path[i], "/", head);
		final_path = ft_strjoin2(path1, cmd, head);
		if (access(final_path, F_OK | X_OK) == 0)
			return (final_path);
		i++;
	}
	return (NULL);
}

char	*direct_execution(char *cmd, char **path, t_data *data)
{
	(void)path;
	if (access(cmd, F_OK ) != 0)
	{
		perror(cmd);
		exit(127);
	}
	else if (access(cmd, X_OK) != 0)
	{
		perror(cmd);
		exit(126);
	}
	return (ft_strdup(cmd, data));
}

char	*get_path(char **env, char *cmp, t_data *data, t_alloc **head)
{
    char	**path;
    char	*final_path;
    struct stat path_stat;

    if (!cmp || !*cmp)
        return (NULL);
    if (stat(cmp, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		print_error_status(126, cmp, "Is a directory");
    path = env_path(env, data);
    if (cmp[0] == '.' && cmp[1] == '/')
        return (direct_execution(cmp, path, data));
    if (cmp[0] == '/')
        return (direct_execution(cmp, path, data));
    final_path = find_path(path, cmp, data, head);
    return (final_path);
}
