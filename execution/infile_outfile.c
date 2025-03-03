/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 22:40:12 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/02 15:51:09 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void outfile_cas(t_redir *outfiles)
{
	int output;

	if(outfiles->quote != S_QUOTE)
	{
		if (outfiles->name[0] == '$' && ft_strlen(outfiles->name) > 1)
			print_error_status(1, outfiles->name, "ambiguous redirect");
	}
	if(outfiles->type == OUTPUT_DIRECTION)
		output = open(outfiles->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(outfiles->type == OUT_APPEND)
	output = open(outfiles->name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if(!output)
	{
		perror(outfiles->name);
		exit(1);
	}
	close(1);
	dup(output);
	close(output);
}

void infile_cas(t_redir *infiles)
{
    int input;

	if(infiles->quote != S_QUOTE && infiles->type == INPUT_DIRECTION)
	{
		if (infiles->name[0] == '$' && ft_strlen(infiles->name) > 1)
			print_error_status(1, infiles->name, "ambiguous redirect");
	}
	if (infiles->type == INPUT_DIRECTION)
		input = open(infiles->name, O_RDONLY, 0644);
	else
		input = open(infiles->here_doc_filename, O_RDWR, 0644);
	if(input == -1)
	{
		perror(infiles->name);	
		exit(1);
	}
	dup2(input, 0);
	close(input);
}

void	files(t_command *command)
{
	t_redir *files = command->files;

	while(files)
	{
		if (files->type == INPUT_DIRECTION || files->type == HERE_DOC)
			infile_cas(files);
		else if (files->type == OUTPUT_DIRECTION || files->type == OUT_APPEND)
			outfile_cas(files);
		files = files->next;
	}
}
