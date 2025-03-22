/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:39:54 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 19:38:29 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(char **str, t_data *data, t_alloc **head)
{
	t_env	*current;

	current = data->env;
	if (str[1] != NULL)
	{
		write(2, "env: '", 7);
		print_error_status(str[1], "' No such file or directory");
		free_exit_child(data, head, 127);
	}
	while (current)
	{
		if (ft_strchr(current->env_var, '='))
			printf("%s\n", current->env_var);
		current = current->next;
	}
}

void	cd_child(char **str, t_data *data, t_alloc **head)
{
	char	cwd[PATH_MAX];
	char	*target_path;
	char	*path;

	if (!prepare_cd_path(str, data, &target_path))
		free_exit_child(data, head, 1);
	if (chdir(target_path) != 0)
	{
		perror(target_path);
		free_exit_child(data, head, 1);
	}
	path = getcwd(cwd, PATH_MAX);
	if (!path)
	{
		perror("cd: error retrieving current directory:"
			"getcwd: cannot access parent directories");
		free_exit_child(data, head, 1);
	}
	free_exit_child(data, head, 0);
}

void	cd(char **str, t_data *data)
{
	exit_stat(0, 1);
	if (ft_getenv2("PWD", data) != NULL)
	{
		free(data->pwd);
		data->pwd = NULL;
		data->pwd = ftt_strdup(ft_getenv2("PWD", data));
	}
	if (str[1] != NULL && str[2] != NULL)
	{
		printf("cd: too many arguments\n");
		exit_stat(1, 1);
		return ;
	}
	if (str[1] == NULL)
		cd_to_home(data);
	else
		cd_to_path(data, str[1]);
}

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
