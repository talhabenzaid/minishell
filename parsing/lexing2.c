/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:11:08 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 18:05:25 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operation(int i)
{
	return (i == INPUT_DIRECTION || i == OUTPUT_DIRECTION
		|| i == OUT_APPEND || i == HERE_DOC || i == PIPE);
}

int	write_here_doc_lex(t_redir *current, t_data *data)
{
	char	*line;
	char	*limiter;
	int		fd;

	limiter = current->name;
	current->here_doc_filename = get_heredoc_name(data);
	fd = open(current->here_doc_filename, O_RDWR | O_CREAT | O_TRUNC, 0640);
	if (fd == -1)
		return (perror("open heredoc:"), -1);
	unlink(current->here_doc_filename);
	while (1)
	{
		line = readline("> ");
		if (line != NULL)
			add_data_line(line, data);
		if (line == NULL || ft_strcmp(line, limiter) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	return (0);
}

void	open_heredocs_lex(t_redir **head, t_data *data)
{
	t_redir	*current;

	current = *head;
	while (current)
	{
		if (write_here_doc_lex(current, data) == -1)
			return ;
		if (g_in_readline == 4)
			return ;
		current = current->next;
	}	
}

static	void	init_heredoc(t_redir *new)
{
	new->next = NULL;
	new->here_doc_expandable = 1;
	new->here_doc_filename = NULL;
	new->here_doc_trim = 0;
	new->quote = 0;
	new->ambigious = 0;
	new->here_doc_fd = -1;
	new->type = HERE_DOC;
}

void	handle_heredocs(t_token **tokens, t_data *data)
{
	t_redir	*head;
	t_redir	*new;
	int		i;

	i = 0;
	head = NULL;
	while (tokens[i])
	{
		if (tokens[i] && tokens[i]->type == HERE_DOC)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
			{
				i++;
				new = ft_malloc(sizeof(t_redir), data);
				new->name = get_full_name(tokens, &i, data);
				init_heredoc(new);
				lstadd(&head, new);
			}
			else
				i++;
		}
		else
			i++;
	}
	open_heredocs_lex(&head, data);
}
