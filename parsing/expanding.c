/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:19:20 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/02 15:22:00 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redir(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND );
}

char	*expand_token(char *token, t_data *data)
{
	char	*previous;
	int		i;
	int		count;

	previous = NULL;
	while (ft_strchr(token, '$'))
	{
		count = 0;
		i = 0;
		while (token[i] && token[i] != '$')
			i++;
		while (token[count + i] && token[count + i] == '$')
			count++;
		if (count == 1 && token[i] && !token[i + count])
			break ;
		if (count > 1)
			token = handle_multiple_dollars(token, count, i, data);
		else
			token = build_exp_str(token, previous, i + count, data);
	}
	return (token);
}

void handle_redirect(t_token **tokens, int *i, t_data *data)
{
    if (!tokens[*i])
        return;
    if (ft_strlen(tokens[*i]->content) == 1)
    {
		if (tokens[*i + 1] && tokens[*i + 1]->part_of_previous)
		{
			tokens[*i + 1]->part_of_previous = 0;
			remove_token(tokens, *i);
			if (*i > 0)
				(*i)--;
		}
		else
			(*i)++;
    }
    else
    {
        char *original = ft_strdup(tokens[*i]->content, data);
        if (ft_strchr(tokens[*i]->content, '$'))
            tokens[*i]->content = expand_token(tokens[*i]->content, data);
        if (ft_strlen(tokens[*i]->content) == 0)
        {
            if (tokens[*i + 1] && tokens[*i + 1]->part_of_previous)
            {
                tokens[*i + 1]->part_of_previous = 0;
                remove_token(tokens, *i);
                if (*i > 0)
                    (*i)--;
            }
            else
                tokens[(*i)++]->content = original;
        }
        else
            (*i)++;
    }
}

int skip_ops_except_pipe(t_token **tokens, int *current, t_data *data)
{
    if (*current <= 0 || !tokens[*current])
        return (0);
    if (*current > 0 && tokens[*current - 1]->type == HERE_DOC)
    {
        (*current)++;
        while(tokens[*current] && tokens[*current]->part_of_previous)
            (*current)++;
        return (1);
    }
    if (*current > 0 && is_redir(tokens[*current - 1]->type))
    {
        while(tokens[*current] && !is_operation(tokens[*current]->type))
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
		tokens[*i]->content = expand_token(tokens[*i]->content, data);
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
		else if(skip_ops_except_pipe(tokens, &i, data))
			continue;
		else if (tokens[i]->quoted == D_QUOTE)
		{
			tokens[i]->split_later = 0;
			tokens[i]->content = expand_token(tokens[i]->content, data);
			i++;
		}
		else if (tokens[i]->quoted == NO_QUOTE)
			handle_no_quote_token(tokens, &i, data);
		else
		{
			tokens[i]->content = handle_quoted_token(tokens[i]->content, data);
			i++;
		}
	}
}
