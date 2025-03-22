/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:23:14 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:19:01 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	w_ch(char c)
{
	return (!to_handle(c) && ft_isprint(c));
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

int	handle_token_2(char *str, int i)
{
	if (w_ch(str[i]) && !is_quote(str[i]))
		while (str[i] && w_ch(str[i + 1]) && !is_quote(str[i + 1]))
			i++;
	if (str[i] == '>' && str[i + 1] == '>')
		i++;
	else if (str[i] == '<' && str[i + 1] == '<')
	{
		if (str[i + 2] == '-')
			i++;
		i++;
	}
	return (i + 1);
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
			i = handle_token_2(str, i);
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
