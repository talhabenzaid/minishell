/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:17:36 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_ops_except_pipe(t_token **tokens, int *current, t_data *data)
{
	if (*current <= 0 || !tokens[*current])
		return (0);
	if (*current > 0 && tokens[*current - 1]->type == HERE_DOC)
	{
		(*current)++;
		while (tokens[*current] && tokens[*current]->part_of_previous)
			(*current)++;
		return (1);
	}
	if (*current > 0 && is_redir(tokens[*current - 1]->type))
	{
		while (tokens[*current] && !is_operation(tokens[*current]->type))
			handle_redirect(tokens, current, data);
		return (1);
	}
	return (0);
}

void	remove_token(t_token **tokens, int index)
{
	int	i;

	i = index;
	while (tokens[i])
	{
		tokens[i] = tokens[i + 1];
		i++;
	}
}

void	handle_no_quote_token(t_token **tokens, int *i, t_data *data)
{
	tokens[*i]->split_later = 1;
	if (ft_strlen(tokens[*i]->content) == 1 && tokens[*i + 1]
		&& tokens[*i + 1]->part_of_previous)
	{
		tokens[*i + 1]->part_of_previous = 0;
		remove_token(tokens, *i);
		if (*i > 0)
			(*i)--;
	}
	else
	{
		tokens[*i]->content = expand_token2(tokens + *i,
				tokens[*i]->content, data);
		if (ft_strlen(tokens[*i]->content) == 0)
		{
			if (tokens[*i + 1] && tokens[*i + 1]->part_of_previous)
				tokens[*i + 1]->part_of_previous = 0;
			remove_token(tokens, *i);
			if (*i > 0)
				(*i)--;
		}
		else
			(*i)++;
	}
}

void	expanding(t_data *data)
{
	int		i;
	t_token	**tokens;

	tokens = data->tokens;
	i = 0;
	while (tokens[i])
	{
		if (!tokens[i]->expandable)
			i++;
		else if (skip_ops_except_pipe(tokens, &i, data))
			continue ;
		else if (tokens[i]->quoted == D_QUOTE)
		{
			tokens[i]->split_later = 0;
			tokens[i]->content = expand_token(tokens[i]->content, data);
			i++;
		}
		else if (tokens[i]->quoted == NO_QUOTE)
			handle_no_quote_token(tokens, &i, data);
	}
}

char	*build_exp_str(char *token, char *previous, int *i, t_data *data)
{
	char	*next;
	char	*expanded;
	int		j;
	int		k;

	k = 0;
	expanded = get_expanded_value(token, *i, &j, data);
	previous = ft_malloc(sizeof(char) * (*i), data);
	ft_strlcpy(previous, token, *i);
	while (token[*i + j + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + *i + j, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	if (ft_strlen(token) == 0)
		*i = 0;
	else if (expanded && ft_strlen(expanded) == 1 && expanded[0] == '$')
		(*i)++;
	else
		*i = ft_strlen(previous) + ft_strlen(expanded);
	return (token);
}
