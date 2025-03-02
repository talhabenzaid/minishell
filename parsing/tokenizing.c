/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:33:05 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/01 21:21:29 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_previous_part(t_token *token, int *i, char *str, int quote)
{
	token->part_of_previous = 0;
	if ((quote == D_QUOTE || quote == S_QUOTE) && *i - 1 > 0
		&& w_ch(str[*i - 2]))
		token->part_of_previous = 1;
	else if (quote == NO_QUOTE && *i > 0 && w_ch(str[*i - 1]))
		token->part_of_previous = 1;
}

static int	get_word_length(char *str, int *i, int quote)
{
	int	j;

	j = 0;
	if (quote == S_QUOTE)
		while (str[*i + j] && str[*i + j] != '\'')
			j++;
	else if (quote == D_QUOTE)
		while (str[*i + j] && str[*i + j] != '\"')
			j++;
	else
		j = count_no_quotes(str + *i);
	return (j);
}

t_token	*insert_token_arr_word(int *i, char *str, int quote, t_data *data)
{
	char		*new;
	int			j;
	t_token		*token;

	token = ft_malloc(sizeof(t_token), data);
	set_previous_part(token, i, str, quote);
	j = get_word_length(str, i, quote);
	new = ft_malloc(sizeof(char) * (j + 1), data);
	ft_strlcpy(new, str + *i, j + 1);
	token->expandable = ((quote == D_QUOTE || quote == NO_QUOTE)
			&& ft_strchr(new, '$'));
	token->content = new;
	token->type = WORD;
	token->quoted = quote;
	token->split_later = 0;
	*i += j;
	return (token);
}

void	loop_token_arr(char *str, t_token **token_arr, t_data *data)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!str[++i])
				break ;
			if (str[i - 1] == '\'')
				token_arr[j++] = insert_token_arr_word(&i, str, S_QUOTE, data);
			else if (str[i - 1] == '\"')
				token_arr[j++] = insert_token_arr_word(&i, str, D_QUOTE, data);
			i++;
		}
		else if (w_ch(str[i]) && !is_quote(str[i]))
			token_arr[j++] = insert_token_arr_word(&i, str, NO_QUOTE, data);
		else if (to_handle(str[i]))
			token_arr[j++] = insert_token_arr_op(&i, str + i, data);
		else
			i++;
	}
	token_arr[j] = NULL;
}

void	tokenize(t_data *data, char *cmd)
{
	int		tokens_count;

	tokens_count = count_tokens(cmd);
	data->token_size = tokens_count;
	data->tokens = ft_malloc(sizeof(t_token *) * (tokens_count + 1), data);
	loop_token_arr(cmd, data->tokens, data);
}
