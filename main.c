/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:46:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/03 01:47:09 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear_env(t_env **lst)
{
	t_env	*tmp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->env_var);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	free_exit(t_data *data)
{
	printf("exit\n");
	ft_lstclear_garbage(&data->alloc);
	ft_lstclear_env(&data->env);
	free(data);
	exit(1);
}

t_data	*env_init(char **env, int argc, char **argv)
{
	t_data	*data;
	t_env *env_list;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if(!data)
		exit(1) ;
	env_list = NULL;
    addenv(env, &env_list);
	data->exit_status = 0;
	data->commands = NULL;
	data->line = NULL;
	data->tokens = NULL;
	data->alloc = NULL;
	if (*env == NULL)
	{
		data->env = NULL;
		data->default_path = strdup("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		if (data->default_path == NULL)
			free_exit(data);
	}
	else
		data->env = env_list;
	return (data);
}


static void	process_command(t_data *data, char *cmd)
{
	if (check_quotes(cmd, data) != 1)
	{
		tokenize(data, cmd);
		if (lexing(data->tokens, data) == 1)
		{
			
			expanding(data);
			parse(data);
			execute(data);
		}
	}
	if (ft_strlen(cmd) != 0)
		add_history(cmd);
}

void	add_data_line(char *str, t_data *data)
{
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
		free_exit(data);
	new->addr = str;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
}

static void	read_command(t_data *data)
{
	char	*cmd;
	char	**lines;
	int		i;

	i = 0;
	data->alloc = NULL;
	g_in_readline = 1;
	data->line = readline("-> minihell ");
	if (!data->line)
		free_exit(data);
	g_in_readline = 0;
	add_data_line(data->line, data);
	lines = ft_split(data->line, '\n', data);
	while(lines[i])
	{
		cmd = ft_strtrim(lines[i], data);
		if (ft_strlen(cmd) == 0 && ft_strlen(lines[i]) > 0)
			add_history(lines[i]);
		if (cmd && ft_strlen(cmd) > 1)
			process_command(data, cmd);
		i++;
	}
}
int g_in_readline = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	data = env_init(env, argc, argv);
	while (1)
	{
		read_command(data);
		ft_lstclear_garbage(&data->alloc);
	}
	free_exit(data);
	return (0);
}
