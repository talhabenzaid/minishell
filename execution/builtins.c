/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:13:56 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/11 16:46:25 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_print(char **str, int i, int option_flag)
{
	while (str[i])
	{
		printf("%s", str[i]);
		if (str[i + 1])
			printf(" ");
		i++;
	}
	if (option_flag == 0)
		printf("\n");
}

void	echo(char **str)
{
	int	i;
	int	option_flag ;
	int	j;

	i = 1;
	option_flag = 0;
	if (str[1] == NULL)
	{
		printf("\n");
		return ;
	}
	while (str[i] && str[i][0] == '-' && str[i][1] == 'n')
	{
		j = 2;
		while (str[i][j] == 'n')
			j++;
		if (str[i][j] == '\0')
		{
			option_flag = 1;
			i++;
		}
		else
			break ;
	}
	echo_print(str, i, option_flag);
}

void	pwd(t_data *data)
{
	char	s[PATH_MAX];
	char	*path;
	char	*test;

	path = getcwd(s, sizeof(s));
	if (!path)
	{
		test = data->pwd;
		if (!test)
			perror("getcwd");
		else
			printf("%s\n", test);
	}
	else
		printf("%s\n", path);
}

void	remove_env_var(char *var_name, t_data *data)
{
	t_env	*prev;
	t_env	*current ;

	prev = NULL;
	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, var_name) == 0)
		{
			if (!prev)
				data->env = current->next;
			else
				prev->next = current->next;
			free(current->env_var);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

void	unset(char **str, t_data *data)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (data->default_path && ft_strcmp(str[i], "PATH") == 0)
		{
			free(data->default_path);
			data->default_path = NULL;
		}
		else
			remove_env_var(str[i], data);
		i++;
	}
}
