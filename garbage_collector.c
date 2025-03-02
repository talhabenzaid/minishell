/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:30:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/02 13:55:01 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	*lst = NULL;
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
		free_exit(data);
	allocated = malloc(size);
	if (allocated == NULL)
	{
		free(new);
		free_exit(data);
	}
	new->addr = allocated;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
	return(allocated);
}
