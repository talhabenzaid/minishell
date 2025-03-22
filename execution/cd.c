/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:22:33 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 18:11:24 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_home(t_env *env)
{
	while (env != NULL)
	{
		if (ft_strcmp_env(env->env_var, "HOME") == 0)
			return (env->env_var + 5);
		env = env->next;
	}
	return (NULL);
}

void	cd_to_home(t_data *data)
{
	char	*home;
	char	*prev_wd;

	prev_wd = data->pwd;
	home = get_home(data->env);
	if (!home)
	{
		printf("cd: HOME not set\n");
		exit_stat(1, 1);
		return ;
	}
	if (ft_strlen(home) == 0)
		exit_stat(0, 1);
	else if (chdir(home) == 0)
		exit_stat(update_pwd_and_oldpwd(data, NULL, prev_wd), 1);
	else
	{
		perror(home);
		exit_stat(1, 1);
	}
}

void	cd_to_path(t_data *data, char *path)
{
	char	*prev_wd;
	char	s[PATH_MAX];

	prev_wd = data->pwd;
	if (prev_wd == NULL)
		prev_wd = getcwd(s, sizeof(s));
	if (chdir(path) == 0)
		exit_stat(update_pwd_and_oldpwd(data, path, prev_wd), 1);
	else
	{
		perror(path);
		exit_stat(1, 1);
	}
}

int	prepare_cd_path(char **str, t_data *data, char **target_path)
{
	if (str[1] == NULL)
	{
		*target_path = get_home(data->env);
		if (*target_path == NULL)
		{
			write(2, "cd: HOME not set\n", 18);
			return (0);
		}
	}
	else
	{
		if (str[2] != NULL)
		{
			write(2, "cd: too many arguments\n", 24);
			return (0);
		}
		*target_path = str[1];
	}
	return (1);
}
