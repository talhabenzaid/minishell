/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 23:52:26 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 17:49:08 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_exit2(t_data *data, int i)
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
	exit(i);
}

void	free_exit(t_data *data)
{
	int	i;

	i = 0;
	if (data->fds)
	{
		while (i < data->fd_count)
		{
			if (data->fds[i] != -1)
				close(data->fds[i]);
			i++;
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
	write(1, "exit\n", 6);
	exit(exit_stat(0, 0));
}

void	destroy_heredocs(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->fd_count)
	{
		if (data->fds[i] != -1)
			close(data->fds[i]);
		i++;
	}
	data->fd_count = 0;
	data->fds = NULL;
}

void	add_data_line(char *str, t_data *data)
{
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
	{
		free(str);
		free_exit(data);
	}
	new->addr = str;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
}
