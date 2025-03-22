/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:40:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 23:26:25 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
			{
				printf("Syntax error unclosed quote\n");
				exit_stat(2, 1);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	err(t_token *prev, t_token *curr)
{
	if (prev->type != PIPE
		|| (prev->type == PIPE && curr->type == PIPE))
	{
		printf("syntax error near unexpected token '%s'\n", curr->content);
		exit_stat(2, 1);
		return (1);
	}
	return (0);
}

int	check_consecutive_expressions(t_token **tokens)
{
	int		i;
	t_token	*previous;

	i = 0;
	previous = tokens[i++];
	while (tokens[i])
	{
		if (is_operation(previous->type) && is_operation(tokens[i]->type))
			if (err(previous, tokens[i]))
				return (1);
		previous = tokens[i];
		i++;
	}
	return (0);
}

int	check_end_and_pipe(t_token **tokens, t_data *data)
{
	int	i;

	i = data->token_size;
	if (tokens[0]->type == PIPE)
	{
		printf("synatx error near unexpected token '|'\n");
		exit_stat(2, 1);
		return (1);
	}
	if (is_operation(tokens[i - 1]->type))
	{
		handle_heredocs(tokens, data);
		if (g_in_readline == 4)
			return (1);
		printf("syntax error near unexpected token 'newline'\n");
		exit_stat(2, 1);
		return (1);
	}
	return (0);
}

int	lexing(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	if (tokens[i] == NULL)
		return (0);
	if (check_consecutive_expressions(tokens) == 1)
		return (0);
	if (check_end_and_pipe(tokens, data) == 1)
		return (0);
	return (1);
}
