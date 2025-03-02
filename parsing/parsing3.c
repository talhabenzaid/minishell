/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:38:33 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/02 13:31:00 by tbenzaid         ###   ########.fr       */
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

static char	*get_heredoc_name(t_data *data)
{
	static int	i = 0;
	char		*number;
	char		*name;

	number = ft_itoa(i, data);
	name = ft_strjoin("/tmp/.here_doc_limit", number, data);
	i++;
	return (name);
}

void	here_doc(t_redir *infile, t_data *data)
{
	int		fd;
	char	*line;
	char	*limiter;

	limiter = infile->name;
	infile->here_doc_filename = get_heredoc_name(data);
	fd = open(infile->here_doc_filename, O_RDWR | O_CREAT | O_TRUNC, 0640);
	if (fd == -1)
		return ;
	while (1)
	{
		line = readline("> ");
		add_data_line(line, data);
		if (line == NULL || ft_strcmp(line, limiter) == 0)
			break ;
		if (infile->here_doc_expandable)
			line = handle_quoted_token(line, data);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
}
