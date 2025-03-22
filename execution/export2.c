/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 02:08:56 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/06 02:37:46 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*create_combined_string(char *exist, char *new_value, t_data *data)
{
	char	*somme;
	int		len;

	len = ft_strlen(exist) + ft_strlen(new_value);
	somme = ft_malloc(len + 1, data);
	ft_strncpy(somme, exist, ft_strlen(exist));
	ft_strncpy(somme + ft_strlen(exist), new_value, ft_strlen(new_value));
	somme[len] = '\0';
	return (somme);
}

static char	*build_new_env_var(t_env *node, char *somme,
		int key_len, t_data *data)
{
	char	*new;
	int		new_len;

	new_len = key_len + ft_strlen(somme) + 2;
	new = malloc(new_len);
	if (!new)
		free_exit(data);
	ft_strncpy(new, node->env_var, key_len);
	new[key_len] = '=';
	ft_strncpy(new + key_len + 1, somme, ft_strlen(somme));
	new[new_len - 1] = '\0';
	return (new);
}

void	append_env(t_env *node, char *new_value,
		char *current_rest, t_data *data)
{
	char	*exist;
	char	*somme;
	char	*new;
	int		key_len;

	exist = "";
	if (current_rest)
		exist = current_rest + 1;
	somme = create_combined_string(exist, new_value, data);
	if (current_rest)
		key_len = current_rest - node->env_var;
	else
		key_len = ft_strlen(node->env_var);
	new = build_new_env_var(node, somme, key_len, data);
	free(node->env_var);
	node->env_var = new;
}
