/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 00:06:52 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/12 08:03:29 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_if_building(char **args, t_data *data, t_alloc **head)
{
	if (args[0] == NULL)
		free_exit_child(data, head, 0);
	if (ft_strcmp(args[0], "echo") == 0)
	{
		echo(args);
		free_exit_child(data, head, 0);
	}
	if (ft_strcmp(args[0], "pwd") == 0)
	{
		pwd(data);
		free_exit_child(data, head, 0);
	}
	if (ft_strcmp(args[0], "unset") == 0)
		free_exit_child(data, head, 0);
	if (ft_strcmp(args[0], "export") == 0)
		export_child(args, data, head);
	if (ft_strcmp(args[0], "cd") == 0)
		cd_child(args, data, head);
	if (ft_strcmp(args[0], "exit") == 0)
		exit_child(args, data, head);
	if (ft_strcmp(args[0], "env") == 0)
	{
		env(args, data, head);
		free_exit_child(data, head, 0);
	}
}

int	first_child(t_command *command, t_env *env_list, t_data *data)
{
	int		fd[2];
	int		pid;
	char	**env;

	env = convert_env_list_to_array(env_list, data);
	if (pipe(fd) == -1)
		return (exit_stat(1, 1), perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
	{
		exit_stat(1, 1);
		return (perror("fork"), close(fd[0]), close(fd[1]), -1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		if (dup2(fd[1], 1) == -1)
		{
			close(fd[1]);
			free_exit2(data, 1);
		}
		close(fd[1]);
		execute_command(command->args, env, data, command);
	}
	return (close(fd[1]), fd[0]);
}

int	mid_childs(int fd_write, t_command *command, t_data *data, t_env *env_list)
{
	int		fd[2];
	int		pid;
	char	**env;

	env = convert_env_list_to_array(env_list, data);
	if (pipe(fd) == -1)
		return (perror("pipe"), close(fd_write), -1);
	pid = fork();
	if (pid == -1)
	{
		close(fd_write);
		close(fd[1]);
		close(fd[0]);
		return (perror("fork"), -1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		ft_dup2(fd_write, fd[1], data);
		execute_command(command->args, env, data, command);
	}
	close(fd_write);
	close(fd[1]);
	return (fd[0]);
}

void	last_child(int fd_write, t_command *command,
		t_data *data, t_env *env_list)
{
	int		pid;
	int		status;
	char	**env;

	env = convert_env_list_to_array(env_list, data);
	pid = fork();
	if (pid == -1)
	{
		close(fd_write);
		perror("fork");
		exit_stat(1, 1);
		return ;
	}
	if (pid == 0)
	{
		if (dup2(fd_write, 0) == -1)
			return (close(fd_write), free_exit2(data, 1));
		close(fd_write);
		execute_command(command->args, env, data, command);
	}
	close(fd_write);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	exit_status(status);
}

void	pipe_cas(t_command **cmd, t_env *env_list, t_data *data)
{
	int		fd_write;
	int		i;

	i = 0;
	fd_write = first_child(cmd[0], env_list, data);
	if (fd_write == -1)
		return ;
	if (data->command_count >= 3)
	{
		while (i < (data->command_count) - 2)
		{
			fd_write = mid_childs(fd_write, cmd[i + 1], data, env_list);
			if (fd_write == -1)
			{
				exit_stat(1, 1);
				while (wait(NULL) != -1)
					;
				return ;
			}
			i++;
		}
	}
	last_child(fd_write, cmd[data->command_count - 1], data, env_list);
	while (wait(NULL) != -1)
		;
}
