/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:38:33 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 17:59:09 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_full_name(t_token **tokens, int *i, t_data *data)
{
	char	*name;

	name = tokens[*i]->content;
	(*i)++;
	while (tokens[*i] && tokens[*i]->part_of_previous)
	{
		name = ft_strjoin(name, tokens[*i]->content, data);
		(*i)++;
	}
	return (name);
}

static int	count_token_words(t_token *token)
{
	if (token->split_later == 1)
		return (word_count(token->content, ' '));
	return (1);
}

int	arg_n(t_token **tokens, int tokens_count)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < tokens_count)
	{
		if (is_operation(tokens[i]->type))
		{
			i += 2;
			while (i < tokens_count && tokens[i]->part_of_previous)
				i++;
		}
		else
		{
			count += count_token_words(tokens[i]);
			i++;
			while (i < tokens_count && tokens[i]->part_of_previous)
			{
				count += count_token_words(tokens[i]);
				i++;
			}
		}
	}
	return (count);
}
