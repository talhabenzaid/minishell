/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:12:09 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/01 13:43:26 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_status(int status, char *str, const char *msg)
{
	write(2, "minihell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(status);
}

void	exit_status(int status, t_data *data)

{
	if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
}

int execu_cmd(char **str, char **env_list, t_data *data)
{
    int pid;
    char *path;
	int	status;

    pid = fork();
    if(pid < 0)
        return(1);
    if(pid == 0)
    {   
        files(data->commands[0]);
        check_if_building(str,data->env,data);
        path = get_path(env_list,str[0], data);
        if(!path)
		{
			access(str[0], F_OK | X_OK);
			perror(str[0]);
			exit(127);
		}
        if (execve(path, str, env_list) == -1)
        {
			access(str[0], F_OK | X_OK);
            perror(str[0]);
            exit(1);
        }
    }
    else
	{
        wait(&status);
		exit_status(status, data);
	}
    return(0);
}

void execution_cas(char **args, t_env *env_list, t_data *data)
{
    char **envs = convert_env_list_to_array(env_list, data);
    if (ft_strcmp(args[0], "cd") == 0)
        cd(args, data);
    else if (ft_strcmp(args[0], "exit") == 0)
        exit_program(args, data);
	else if (ft_strcmp(args[0], "export") == 0 && args[1] != NULL)
		export(args, data);
    else if (ft_strcmp(args[0], "unset") == 0)
        unset(args, data);
    else
        execu_cmd(args, envs, data);
}

void addenv(char **env, t_env **head)
{
    if (!env || !*env)
	{
		
        return;
	}
    int i = 0;
    while (env[i])
    {
        t_env *node = malloc(sizeof(t_env));
        node->env_var = strdup(env[i]);
        node->next = NULL;
        if (!*head)
            *head = node;
        else {
            t_env *temp = *head;
            while (temp->next)
                temp = temp->next;
            temp->next = node;
        }
        i++;
    }
}

char **convert_env_list_to_array(t_env *head, t_data *data)
{
    int count = 0;
    t_env *temp = head;

    while (temp) {
        count++;
        temp = temp->next;
    }
    char **envs = ft_malloc((count + 1) * sizeof(char *), data);
    temp = head;
    int i = 0;
    while (temp) {
        envs[i] = ft_strdup(temp->env_var, data);
        temp = temp->next;
        i++;
    }
    envs[i] = NULL;
    return envs;
}

int    execute(t_data *data)
{
    if(data->command_count == 1)
    {
        t_command *cmd = data->commands[0];
        execution_cas(cmd->args, data->env, data);
    }
    else 
        pipe_cas(data->commands,data->env,data);
    return 0;
}
