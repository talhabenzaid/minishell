/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile_parent.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:05:04 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 18:10:41 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	int	check_dir(char *str)
{
	struct stat	path_stat;

	if (stat(str, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_error_status(str, "Is a directory");
		return (1);
	}
	return (0);
}

int	outfile_cas_p(t_redir *outfiles)
{
	int	output;

	if (outfiles->ambigious)
	{
		print_error_status(outfiles->name, "ambiguous redirect");
		return (0);
	}
	if (check_dir(outfiles->name))
		return (0);
	if (outfiles->type == OUTPUT_DIRECTION)
		output = open(outfiles->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfiles->type == OUT_APPEND)
		output = open(outfiles->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (output == -1)
	{
		write(2, "minihell: ", 11);
		perror(outfiles->name);
		return (0);
	}
	close(output);
	return (1);
}

int	infile_cas_p(t_redir *infiles)
{
	int	input;

	if (infiles->ambigious && infiles->type == INPUT_DIRECTION)
	{
		print_error_status(infiles->name, "ambiguous redirect");
		return (0);
	}
	if (infiles->type == INPUT_DIRECTION)
		input = open(infiles->name, O_RDONLY, 0644);
	else
		input = infiles->here_doc_fd;
	if (input == -1)
	{
		write(2, "minihell: ", 11);
		perror(infiles->name);
		return (0);
	}
	close(input);
	return (1);
}

int	files_p(t_command *command)
{
	t_redir	*files;

	files = command->files;
	while (files)
	{
		if (files->type == INPUT_DIRECTION || files->type == HERE_DOC)
		{
			if (infile_cas_p(files) == 0)
			{
				exit_stat(1, 1);
				return (1);
			}
		}
		else if (files->type == OUTPUT_DIRECTION || files->type == OUT_APPEND)
		{
			if (outfile_cas_p(files) == 0)
			{
				exit_stat(1, 1);
				return (1);
			}
		}
		files = files->next;
	}
	return (0);
}
