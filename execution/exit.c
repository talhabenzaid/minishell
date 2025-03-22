/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:30:41 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/15 20:55:38 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long	ft_atol(char *str, t_data *data)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
			(printf("exit\nexit: %s: numeric argument required\n", str),
				free_exit2(data, 2));
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	ft_atol2(char *str, t_data *data, t_alloc **head)
{
	long	result;
	int		sign;
	int		i;

	(1) && (result = 0, sign = 1, i = 0);
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			print_error_status(str, ":numeric argument required");
			free_exit_child(data, head, 2);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	valid(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_child(char **str, t_data *data, t_alloc **head)
{
	if (str[1] == NULL)
	{
		handle_exit(data, head, 0, 1);
		return ;
	}
	if (str[2] != NULL)
	{
		if (!valid(str[1]))
		{
			print_invalid_arg_msg(str[1], 1);
			handle_exit(data, head, 2, 1);
		}
		print_too_many_args(1);
		handle_exit(data, head, 1, 1);
	}
	if (!valid(str[1]))
	{
		print_invalid_arg_msg(str[1], 1);
		handle_exit(data, head, 2, 1);
	}
	handle_exit(data, head, ft_atol2(str[1], data, head) % 256, 1);
}

void	exit_program(char **str, t_data *data)
{
	if (str[1] == NULL)
	{
		handle_exit(data, NULL, 0, 0);
		return ;
	}
	if (str[2] != NULL)
	{
		if (!valid(str[1]))
		{
			print_invalid_arg_msg(str[1], 0);
			handle_exit(data, NULL, 2, 0);
		}
		print_too_many_args(0);
		handle_exit(data, NULL, 1, 0);
		return ;
	}
	if (!valid(str[1]))
	{
		print_invalid_arg_msg(str[1], 0);
		handle_exit(data, NULL, 2, 0);
	}
	handle_exit(data, NULL, ft_atol(str[1], data) % 256, 0);
}
