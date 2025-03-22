/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 05:37:40 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 18:14:26 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_invalid_arg_msg(char *arg, int is_child)
{
	if (is_child)
	{
		write(2, "exit: ", 7);
		print_error_status(arg, ": numeric argument required");
	}
	else
		printf("exit\nexit: %s: numeric argument required\n", arg);
}

void	print_too_many_args(int is_child)
{
	if (is_child)
		write(2, "exit: too many arguments\n", 26);
	else
		printf("exit\nexit: too many arguments\n");
}

void	handle_exit(t_data *data, t_alloc **head, int status, int is_child)
{
	if (is_child)
		free_exit_child(data, head, status);
	else
	{
		if (status != 1 || head != NULL)
			free_exit2(data, status);
		else
			exit_stat(1, 1);
	}
}
