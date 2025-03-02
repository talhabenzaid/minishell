/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:38:17 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/23 19:32:17 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_spaces(int count, t_data *data)
{
	char	*str;
	int		i;

	str = ft_malloc(sizeof(char) * (count + 1), data);
	i = 0;
	while (i < count)
		str[i++] = ' ';
	str[i] = '\0';
	return (str);
}

char	*join_tokens(char **strs, char *token, int j, t_data *data)
{
	char	*temp;
	int		i;

	i = 1;
	temp = ft_strjoin(create_spaces(j, data), strs[0], data);
	while (strs[i])
	{
		while (token[j] && token[j] != ' ')
			j++;
		token = token + j;
		j = 0;
		while (token[j] && token[j] == ' ')
			j++;
		temp = ft_strjoin(temp, create_spaces(j, data), data);
		temp = ft_strjoin(temp, strs[i], data);
		i++;
	}
	return (temp);
}

char	*handle_quoted_token(char *token, t_data *data)
{
	char	**strs;
	int		i;
	int		j;

	j = 0;
	i = 0;
	strs = ft_split(token, ' ', data);
	while (strs[i])
	{
		if (ft_strchr(strs[i], '$'))
			strs[i] = expand_token(strs[i], data);
		i++;
	}
	while (token[j] && token[j] == ' ')
		j++;
	if (i == 1)
		return (ft_strjoin(create_spaces(j, data), strs[0], data));
	return (join_tokens(strs, token, j, data));
}
