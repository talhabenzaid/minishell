/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:41:57 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 20:55:06 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**convert_env_list_to_array(t_env *head, t_data *data)
{
	t_env	*temp;
	int		i;
	char	**envs;

	i = 0;
	temp = head;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	envs = ft_malloc((i + 1) * sizeof(char *), data);
	i = 0;
	while (head)
	{
		envs[i] = ft_strdup(head->env_var, data);
		head = head->next;
		i++;
	}
	return (envs[i] = NULL, envs);
}
