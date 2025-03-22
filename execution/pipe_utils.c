/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:43:05 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 19:42:41 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (fd < 0 || s == NULL)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	handle_cmd_error(char *cmd, t_data *data, t_alloc **head)
{
	if (access(cmd, F_OK) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else
		perror(cmd);
	free_exit_child(data, head, 127);
}

void	execute_command(char **argv, char **env,
		t_data *data, t_command *command)
{
	char	*path;
	t_alloc	*head;

	head = NULL;
	files(command, data, &head);
	check_if_building(argv, data, &head);
	path = get_path(env, argv[0], data, &head);
	if (!path)
		handle_cmd_error(argv[0], data, &head);
	destroy_heredocs(data);
	if (execve(path, argv, env) == -1)
	{
		if (access(argv[0], F_OK | X_OK) != -1)
			free_exit_child(data, &head, 0);
		perror("execve");
		free_exit_child(data, &head, 1);
	}
}

void	ft_dup2(int input, int output, t_data *data)
{
	if (dup2(input, 0) == -1)
	{
		perror("dup2");
		close(input);
		close(output);
		free_exit2(data, 1);
	}
	close(input);
	if (dup2(output, 1) == -1)
	{
		perror("dup2");
		close(output);
		free_exit2(data, 1);
	}
	close(output);
}

void	free_exit_child(t_data *data, t_alloc **head, int i)
{
	int	j;

	j = 0;
	if (data->fds)
	{
		while (j < data->fd_count)
		{
			if (data->fds[j] != -1)
				close(data->fds[j]);
			j++;
		}
	}
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
	if (data->default_path)
		free(data->default_path);
	if (data->pwd)
		free(data->pwd);
	if (data->fd_write != -1)
		close(data->fd_write);
	ft_lstclear_garbage(head);
	exit(i);
}
