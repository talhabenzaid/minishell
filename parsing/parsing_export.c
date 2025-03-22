/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:51:57 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:40:05 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_export(t_data *data, t_token **tokens, t_command *command)
{
	int	i;

	i = 0;
	command->is_export = 0;
	while (tokens[i])
	{
		if (is_operation(tokens[i]->type) && tokens[i]->type != PIPE)
		{
			i += 2;
			while (tokens[i] && tokens[i]->part_of_previous)
				i++;
		}
		else if (tokens[i]->type == WORD)
		{
			if (ft_strcmp("export", get_full_name(tokens, &i, data)) == 0)
				command->is_export = 1;
			return ;
		}
		else if (tokens[i]->type == PIPE)
			return ;
	}
}

int	fih_space(const char *str)
{
	int	i;
	int	equal_i;

	i = 0;
	equal_i = -1;
	while (str[i])
	{
		if (str[i] == '=')
		{
			equal_i = i;
			break ;
		}
		i++;
	}
	if (equal_i == -1)
		return (1);
	i = 0;
	while (str[i] && i <= equal_i)
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

static int	count_token_words_export(t_token *token)
{
	if (token->split_later == 1)
	{	
		if (fih_space(token->content))
			return (word_count(token->content, ' '));
	}
	return (1);
}

int	arg_x(t_token **tokens, int tokens_count)
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
			count += count_token_words_export(tokens[i]);
			i++;
			while (i < tokens_count && tokens[i]->part_of_previous)
			{
				count += count_token_words_export(tokens[i]);
				i++;
			}
		}
	}
	return (count);
}

void	command_init(t_command *command)
{
	command->args = NULL;
	command->files = NULL;
	command->is_export = 0;
}
