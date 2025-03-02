/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:23:14 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/01 21:21:54 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	to_handle(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_quotes(char *str, int *i, char quote_char)
{
	int	count;

	count = 0;
	if (str[*i] == quote_char)
	{
		count++;
		(*i)++;
		if (str[*i] == quote_char)
		{
			(*i)++;
			return (count);
		}
		while (str[*i] && str[*i] != quote_char)
			(*i)++;
	}
	if (str[*i] == quote_char)
		(*i)++;
	return (count);
}

int	count_tokens(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			count += handle_quotes(str, &i, str[i]);
		else if ((w_ch(str[i]) || to_handle(str[i])) && !is_quote(str[i]))
		{
			count++;
			if (w_ch(str[i]) && !is_quote(str[i]))
				while (str[i] && w_ch(str[i + 1]) && !is_quote(str[i + 1]))
					i++;
			if (str[i] == '>' && str[i + 1] == '>')
				i++;
			else if (str[i] == '<' && str[i + 1] == '<')
				i++;
			i++;
		}
		else
			i++;
	}
	return (count);
}

int	count_no_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ' ' && w_ch(str[i]) && !is_quote(str[i]))
		i++ ;
	return (i);
}
