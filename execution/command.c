/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:12:09 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 19:38:40 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_status(int status)
{
	if (WIFSIGNALED(status))
		exit_stat(128 + WTERMSIG(status), 1);
	else if (WIFEXITED(status))
		exit_stat(WEXITSTATUS(status), 1);
}

int	execute_child_process(char **str, char **env_list,
		t_data *data, t_alloc **head)
{
	char	*path;

	files(data->commands[0], data, head);
	check_if_building(str, data, head);
	path = get_path(env_list, str[0], data, head);
	if (!path)
	{
		if (access(str[0], F_OK) == -1)
			print_error_status(str[0], ": command not found");
		else
			perror(str[0]);
		free_exit_child(data, head, 127);
	}
	destroy_heredocs(data);
	if (execve(path, str, env_list) == -1)
	{
		if (access(str[0], F_OK | X_OK) != -1)
			free_exit_child(data, head, 0);
		perror("execve");
		free_exit_child(data, head, 1);
	}
	return (0);
}

int	execu_cmd(char **str, char **env_list, t_data *data)
{
	int		pid;
	int		status;
	t_alloc	*head;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		head = NULL;
		execute_child_process(str, env_list, data, &head);
	}
	else
	{
		wait(&status);
		exit_status(status);
	}
	return (0);
}

void	execution_cas(char **args, t_env *env_list, t_data *data)
{
	char	**envs;

	envs = convert_env_list_to_array(env_list, data);
	if (ft_strcmp(args[0], "cd") == 0)
	{
		if (files_p(data->commands[0]) == 0)
			cd(args, data);
	}
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		if (files_p(data->commands[0]) == 0)
			exit_program(args, data);
	}
	else if (ft_strcmp(args[0], "export") == 0 && args[1] != NULL)
	{
		if (files_p(data->commands[0]) == 0)
			export(args, data);
	}
	else if (ft_strcmp(args[0], "unset") == 0)
	{
		if (files_p(data->commands[0]) == 0)
			unset(args, data);
	}
	else
		execu_cmd(args, envs, data);
}

int	execute(t_data *data)
{
	t_command	*cmd;

	if (data->command_count == 1)
	{
		cmd = data->commands[0];
		execution_cas(cmd->args, data->env, data);
	}
	else
		pipe_cas(data->commands, data->env, data);
	return (0);
}
