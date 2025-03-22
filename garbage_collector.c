/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:30:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:58:23 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_stat(int status, int flag)
{
	static int	s = 0;

	if (flag != 0)
		s = status;
	return (s);
}

void	ft_lstclear_garbage(t_alloc **lst)
{
	t_alloc	*tmp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->addr);
		free(*lst);
		*lst = tmp;
	}
}

void	ft_lstadd_front(t_alloc **lst, t_alloc *new)
{
	if (new == NULL || lst == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

void	*ft_malloc(size_t size, t_data *data)
{
	void	*allocated;
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
	{
		write(2, "fatal error: cannot allocate memory\n", 37);
		exit_stat(1, 1);
		free_exit(data);
	}
	allocated = malloc(size);
	if (allocated == NULL)
	{
		write(2, "fatal error: cannot allocate memory\n", 37);
		exit_stat(1, 1);
		free(new);
		free_exit(data);
	}
	new->addr = allocated;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
	return (allocated);
}

void	*ft_malloc2(size_t size, t_data *data, t_alloc **child)
{
	void	*allocated;
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
	{
		write(2, "fatal error: cannot allocate memory\n", 37);
		free_exit_child(data, child, 1);
	}
	allocated = malloc(size);
	if (allocated == NULL)
	{
		free(new);
		write(2, "malloc failure in child\n", 25);
		free_exit_child(data, child, 1);
	}
	new->addr = allocated;
	new->next = NULL;
	ft_lstadd_front(child, new);
	return (allocated);
}
