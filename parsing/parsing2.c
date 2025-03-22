/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:37:25 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/08 22:39:33 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_commands(t_token **tokens)
{
	int	i;
	int	count;

	count = 1;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE)
			count++;
		i++;
	}
	return (count);
}

t_redir	*ft_lstlast(t_redir *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	lstadd(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

char	*get_full_name_in(t_token **tok, t_redir *inf, int *i, t_data *data)
{
	char	*name;

	inf->here_doc_expandable = 1;
	name = tok[*i]->content;
	inf->here_doc_trim = tok[*i - 1]->here_doc_trim;
	if (tok[*i]->quoted)
		inf->here_doc_expandable = 0;
	if (tok[*i]->ambigious)
		inf->ambigious = tok[*i]->ambigious;
	(*i)++;
	while (tok[*i] && tok[*i]->part_of_previous)
	{
		if (tok[*i]->ambigious)
			inf->ambigious = tok[*i]->ambigious;
		name = ft_strjoin(name, tok[*i]->content, data);
		if (tok[*i]->quoted)
			inf->here_doc_expandable = 0;
		(*i)++;
	}
	return (name);
}

t_redir	*insert_file(t_token **tokens, int *i, t_data *data)
{
	t_redir	*infile;

	infile = ft_malloc(sizeof(t_redir), data);
	infile->here_doc_expandable = 0;
	infile->ambigious = 0;
	infile->type = tokens[*i]->type;
	(*i)++;
	infile->quote = tokens[*i]->quoted;
	infile->name = get_full_name_in(tokens, infile, i, data);
	infile->next = NULL;
	return (infile);
}
