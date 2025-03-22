/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:57:59 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 18:04:00 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_heredocs(t_command **commands)
{
	t_redir	*current;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (commands[i])
	{
		current = commands[i]->files;
		while (current)
		{
			if (current->type == HERE_DOC)
				count++;
			current = current->next;
		}
		i++;
	}
	return (count);
}

int	open_heredocs(t_data *data)
{
	t_redir	*current;
	int		i;
	int		j;

	initialize_heredoc_fds(data);
	i = -1;
	j = 0;
	while (data->commands[++i])
	{
		current = data->commands[i]->files;
		while (current)
		{
			if (g_in_readline == 4)
				break ;
			if (current->type == HERE_DOC && g_in_readline != 4)
				if (here_doc(current, &j, data) == -1)
					return (-1);
			current = current->next;
		}
		if (g_in_readline == 4)
			break ;
	}
	return (0);
}

char	*get_heredoc_name(t_data *data)
{
	static int	i = 0;
	char		*number;
	char		*name;

	i = (i + 2748) * 5256 % 2099980;
	number = ft_itoa(i, data);
	name = ft_strjoin("/tmp/.here_doc_limit", number, data);
	if (access(name, F_OK | X_OK) != -1)
	{
		while (access(name, F_OK | X_OK) != -1)
			name = ft_strjoin(name, "t", data);
	}
	i++;
	return (name);
}

void	write_heredoc(int fd, t_redir *infile, t_data *data)
{
	char	*line;
	char	*limiter;

	g_in_readline = 3;
	limiter = infile->name;
	while (1)
	{
		line = readline("> ");
		add_data_line(line, data);
		if (infile->here_doc_trim)
			line = ft_strtrim_tab(line, data);
		if (line == NULL || ft_strcmp(line, limiter) == 0)
			break ;
		if (infile->here_doc_expandable)
			line = expand_token(line, data);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	data->fd_write = -1;
}

int	here_doc(t_redir *infile, int *j, t_data *data)
{
	int		fd;
	int		fd_read;

	infile->here_doc_filename = get_heredoc_name(data);
	fd = open(infile->here_doc_filename, O_RDWR | O_CREAT | O_TRUNC, 0640);
	if (fd == -1)
		return (perror("minihell:"), exit_stat(1, 1), -1);
	fd_read = open(infile->here_doc_filename, O_RDONLY);
	if (fd == -1)
		return (perror("minihell:"), exit_stat(1, 1), close(fd), -1);
	data->fds[*j] = fd_read;
	(*j)++;
	data->fd_write = fd;
	infile->here_doc_fd = fd_read;
	unlink(infile->here_doc_filename);
	write_heredoc(fd, infile, data);
	return (0);
}
