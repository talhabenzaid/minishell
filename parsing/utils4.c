/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:40:51 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/12 19:18:55 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redir(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND);
}

int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
		return (1);
	else
		return (0);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	to_handle(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (c);
	return (0);
}
