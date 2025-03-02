/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 00:06:52 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/01 13:37:22 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void check_if_building(char **args, t_env *env_list,t_data *data)
{
	(void)env_list;
	if(args[0] == NULL)
		exit(0);
    if (ft_strcmp(args[0], "echo") == 0)
    {
        echo(args);
        exit(0);
    }
    else if (ft_strcmp(args[0], "pwd") == 0)
    {
        pwd();
        exit(0);
    }
	else if (ft_strcmp(args[0], "unset") == 0)
    {
        unset(args,data);
        exit(0);
    }
	else if (ft_strcmp(args[0], "export") == 0)
	{
		export(args, data);
		exit(0);
	}
	else if (ft_strcmp(args[0], "cd") == 0)
    {
        cd(args,data);
		exit(0);
    }
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_child(args);
    else if (ft_strcmp(args[0], "env") == 0)
    {
        env(args, data);
        exit(0);
    }
}

int first_child(char **argv,t_env *env_list,t_data *data, t_command *command)
{
    int fd[2];
    int pid;
    char *path;

    char **env = convert_env_list_to_array(env_list, data);
    if (pipe(fd) == -1)
		 exit(1);
    pid = fork();
	if (pid == -1)
		exit(1);
    if (pid == 0)
	{
		(close(fd[0]), dup2(fd[1], 1), close(fd[1]));
		files(command);
        check_if_building(argv,env_list,data);
		path = get_path(env,argv[0],data);
    	if(!path)
			print_error_status(127, argv[0], "Command not found");
        if (execve(path, argv, env) == -1)
        {
            perror(argv[0]);
            exit(1);
        }
	}
	close(fd[1]);
	return (fd[0]);
}

int	mid_childs(int fd_write,char **argv,t_env *env_list,t_data *data, t_command *command)
{
	int		fd[2];
	int		pid;
	char	*path;

    char **env = convert_env_list_to_array(env_list, data);
	if (pipe(fd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
	{
		(close(fd[0]), dup2(fd_write, 0), dup2(fd[1], 1),
			close(fd_write), close(fd[1]));
			files(command);

        check_if_building(argv,env_list,data);
		path = get_path(env,argv[0],data);
    	if(!path)
			print_error_status(127, argv[0], "Command not found");
        if (execve(path, argv, env) == -1)
        {
            perror(argv[0]);
            exit(1);
        }
	}
	(close(fd_write), close(fd[1]));
	return (fd[0]);
}

void	last_child(int fd_write,char **argv,t_env *env_list,t_data *data, t_command *command)
{
	int		pid;
	char	*path;
	int		status;
    char **env = convert_env_list_to_array(env_list, data);
	pid = fork();
	if (pid == -1)
    return ;
	if (pid == 0)
	{
		(dup2(fd_write, 0), close(fd_write));
 		files(command);
        check_if_building(argv, env_list, data);
		path = get_path(env,argv[0],data);
		if(!path)
			print_error_status(127, argv[0], "Command not found");
        if (execve(path, argv, env) == -1)
        {
            perror(argv[0]);
            exit(1);
        }
	}
	close(fd_write);
	waitpid(pid, &status, 0);
	while(wait(NULL) != -1)
		;
	exit_status(status, data);
}

void pipe_cas(t_command **cmd,t_env *env_list,t_data *data)
{
    int fd_write;
    int i;

	i = 0;
	fd_write = first_child(cmd[0]->args,env_list,data, data->commands[0]);
	if(data->command_count >= 3)
	{
		while(i < (data->command_count) - 2)
		{
			fd_write = mid_childs(fd_write,cmd[i + 1]->args,env_list,data, data->commands[i]);
			i++;
		}
	}
	last_child(fd_write,cmd[data->command_count - 1]->args,env_list,data, data->commands[data->command_count - 1]);
}
