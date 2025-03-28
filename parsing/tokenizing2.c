/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:08:56 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/08 21:21:49 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_other_ops(t_token *token, char *str, t_data *data)
{
	char	*new;

	if (str[0] == '|')
		token->type = PIPE;
	else if (str[0] == '<')
		token->type = INPUT_DIRECTION;
	else if (str[0] == '>')
		token->type = OUTPUT_DIRECTION;
	new = ft_malloc(sizeof(char) * 2, data);
	ft_strlcpy(new, str, 2);
	token->content = new;
}

static	void	op_init(t_token *token)
{
	token->here_doc_trim = 0;
	token->quoted = 0;
	token->expandable = 0;
	token->part_of_previous = 0;
	token->ambigious = 0;
}

t_token	*insert_token_arr_op(int *i, char *str, t_data *data)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), data);
	op_init(token);
	if (str[0] == '<' && str[1] == '<')
	{
		token->type = HERE_DOC;
		token->content = ft_strdup("<<", data);
		if (str[2] == '-')
		{
			token->here_doc_trim = 1;
			*i += 1;
		}
		*i += 1;
	}
	else if (str[0] == '>' && str[1] == '>')
	{
		token->type = OUT_APPEND;
		token->content = ft_strdup(">>", data);
		*i += 1;
	}
	else
		insert_other_ops(token, str, data);
	*i += 1;
	return (token);
}
