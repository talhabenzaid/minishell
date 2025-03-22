/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:28:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 18:12:49 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_token(char **token, int count, int *i, t_data *data)
{
	if (count > 1)
		*token = handle_multiple_dollars(*token, count, *i, data);
	else
	{
		(*i)++;
		*token = build_exp_str(*token, NULL, i, data);
	}
}

char	*expand_token2(t_token **t, char *token, t_data *data)
{
	int		i;
	int		n;
	int		l;

	i = 0;
	while (token[i])
	{
		l = ft_strlen(token);
		if (i >= l)
			break ;
		if (token[i] == '$')
		{
			n = 0;
			while ((i + n) < l && token[i + n] == '$')
				n++;
			if (n == 1 && (i + n) >= l && t[1] && t[1]->part_of_previous)
				return (token[i] = '\0', token);
			if (n == 1 && (i + n) >= l)
				break ;
			handle_token(&token, n, &i, data);
		}
		else
			i++;
	}
	return (token);
}

char	*expand_token(char *token, t_data *data)
{
	int		i;
	int		count;
	int		token_len;

	i = 0;
	while (token[i])
	{
		token_len = ft_strlen(token);
		if (i >= token_len)
			break ;
		if (token[i] == '$')
		{
			count = 0;
			while ((i + count) < token_len && token[i + count] == '$')
				count++;
			if (count == 1 && (i + count) >= token_len)
				break ;
			handle_token(&token, count, &i, data);
		}
		else
			i++;
	}
	return (token);
}

char	*get_expanded_value(char *token, int i, int *j, t_data *data)
{
	char	*expanded;

	*j = 0;
	if (token[i] == '?')
		return ((*j)++, ft_itoa(exit_stat(69, 0), data));
	if (!ft_isalnum(token[i]) && token[i] && token[i + *j] != '_')
		return (ft_strdup("$", data));
	else
	{
		if (ft_isdigit(token[i]))
			(*j)++;
		else
		{
			while (token[i + *j] && (token[i + *j] != '$'))
			{
				if (!ft_isalnum(token[i + *j]) && token[i + *j] != '_')
					break ;
				(*j)++;
			}
		}
	}
	expanded = ft_malloc(sizeof(char) * (*j + 1), data);
	ft_strlcpy(expanded, token + i, *j + 1);
	return (ft_getenv(expanded, data));
}

char	*handle_multiple_dollars(char *token, int count, int i, t_data *data)
{
	char	*previous;
	char	*expanded;
	int		k;
	char	*next;

	if (count % 2 != 0)
		count--;
	expanded = ft_strdup("", data);
	previous = ft_malloc(sizeof(char) * i + 1, data);
	ft_strlcpy(previous, token, i + 1);
	k = 0;
	while (token[i + count + k])
		k++;
	next = ft_malloc(sizeof(char) * (k + 1), data);
	ft_strlcpy(next, token + i + count, k + 1);
	token = ft_strjoin(previous, expanded, data);
	token = ft_strjoin(token, next, data);
	return (token);
}
