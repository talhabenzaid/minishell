/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:59:54 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 17:58:07 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*build_command_export(t_command *command, t_token **tokens,
	t_data *data, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	command->args = ft_malloc(sizeof(char *) * (arg_x(tokens, n) + 1), data);
	while (i < n)
	{
		if (is_operation(tokens[i]->type) && tokens[i]->type != HERE_DOC)
			lstadd(&command->files, insert_file(tokens, &i, data));
		else if (tokens[i]->type == HERE_DOC)
			lstadd(&command->files, in_heredoc(tokens, &i, data));
		else if (tokens[i]->type == WORD && tokens[i]->split_later
			&& fih_space(tokens[i]->content))
			splt(get_full_name(tokens, &i, data), command->args, &j, data);
		else if (tokens[i]->type == WORD)
			command->args[j++] = get_full_name(tokens, &i, data);
		else
			i++;
	}
	command->args[j] = NULL;
	command->arg_n = j;
	return (command);
}

t_redir	*in_heredoc(t_token **tokens, int *i, t_data *data)
{
	t_redir	*infile;

	infile = ft_malloc(sizeof(t_redir), data);
	infile->type = tokens[*i]->type;
	(*i)++;
	infile->quote = tokens[*i]->quoted;
	infile->name = get_full_name_in(tokens, infile, i, data);
	infile->next = NULL;
	return (infile);
}

void	splt(char *str, char **command, int *j, t_data *data)
{
	char	**split_str;
	int		i;

	i = 0;
	split_str = ft_split(str, ' ', data);
	while (split_str[i])
	{
		command[*j] = split_str[i];
		i++;
		(*j)++;
	}
}

t_command	*build_command(t_data *data, t_token **tokens, int i, int n)
{
	int			j;
	t_command	*command;

	command = ft_malloc(sizeof(t_command), data);
	command_init(command);
	check_export(data, tokens, command);
	if (command->is_export)
		return (build_command_export(command, tokens, data, n));
	i = 0;
	j = 0;
	command->args = ft_malloc(sizeof(char *) * (arg_n(tokens, n) + 1), data);
	while (i < n)
	{
		if (is_operation(tokens[i]->type) && tokens[i]->type != HERE_DOC)
			lstadd(&command->files, insert_file(tokens, &i, data));
		else if (tokens[i]->type == HERE_DOC)
			lstadd(&command->files, in_heredoc(tokens, &i, data));
		else if (tokens[i]->type == WORD && tokens[i]->split_later)
			splt(get_full_name(tokens, &i, data), command->args, &j, data);
		else if (tokens[i]->type == WORD)
			command->args[j++] = get_full_name(tokens, &i, data);
		else
			i++;
	}
	return (command->args[j] = NULL, command->arg_n = j, command);
}

int	parse(t_data *data)
{
	int		i;
	int		j;
	t_token	**tokens;

	data->command_count = count_commands(data->tokens);
	i = 0;
	tokens = data->tokens;
	data->commands = ft_malloc(sizeof(t_command *)
			* (data->command_count + 1), data);
	while (i < data->command_count)
	{
		j = 0;
		while (tokens[j] && tokens[j]->type != PIPE)
			j++;
		data->commands[i++] = build_command(data, tokens, 0, j);
		if (tokens[j] && tokens[j]->type == PIPE)
			j++;
		tokens = tokens + j;
	}
	data->commands[i] = NULL;
	if (open_heredocs(data) == -1)
		return (-1);
	return (0);
}
