/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:01:09 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 20:55:14 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	update_oldpwd(t_data *data, char *prev_wd)
{
	int	flag;

	flag = 0;
	if (ft_getenv2("PWD", data) == NULL && ft_getenv2("OLDPWD", data) != NULL)
	{
		remove_env_var("OLDPWD", data);
		check_add("OLDPWD", data);
		flag = 1;
	}
	else
		check_add(ft_strjoin("OLDPWD=", prev_wd, data), data);
	return (flag);
}

static int	handle_pwd_error(t_data *data, char *path3, char *prev_wd, int flag)
{
	char	*path;
	char	*path2;

	if (prev_wd == NULL)
		return (1);
	if (prev_wd[ft_strlen(prev_wd) - 1] != '/')
		path = ft_strjoin(prev_wd, "/", data);
	else
		path = prev_wd;
	path2 = ft_strjoin(path, path3, data);
	free(data->pwd);
	data->pwd = ftt_strdup(path2);
	if (!data->pwd)
		free_exit(data);
	if (flag == 0)
		check_add(ft_strjoin("PWD=", path2, data), data);
	return (1);
}

static int	update_pwd(t_data *data, char *path3, char *prev_wd, int flag)
{
	char	cwd[PATH_MAX];
	char	*path;

	path = getcwd(cwd, PATH_MAX);
	if (!path)
	{
		perror("cd: error retrieving current directory: "
			"getcwd: cannot access parent directories");
		return (handle_pwd_error(data, path3, prev_wd, flag));
	}
	if (flag == 0)
		check_add(ft_strjoin("PWD=", path, data), data);
	return (0);
}

int	update_pwd_and_oldpwd(t_data *data, char *path3, char *prev_wd)
{
	int	flag;

	flag = update_oldpwd(data, prev_wd);
	return (update_pwd(data, path3, prev_wd, flag));
}
