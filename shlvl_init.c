/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 19:30:43 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 19:59:17 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_non_digits(char *str)
{
	int	sign;

	sign = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
	{
		str++;
		sign = 1;
	}
	if (!(*str >= '0' && *str <= '9') && sign)
		return (1);
	while (*str >= '0' && *str <= '9')
		str++;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str != '\0')
		return (1);
	return (0);
}

void	shlvl_warning(int number)
{
	write(2, "minihell: warning: shell level (", 33);
	ft_putnbr_fd(number, 2);
	write(2, ") too high, resetting to 1\n", 28);
}

static	void	update_shlvl(char *new_number, t_data *data)
{
	t_env	*current;

	current = data->env;
	while (current)
	{
		if (ft_strcmp_env(current->env_var, "SHLVL") == 0)
			break ;
		current = current->next;
	}
	free(current->env_var);
	current->env_var = ftt_strdup(ft_strjoin("SHLVL=", new_number, data));
	if (current->env_var == NULL)
		free_exit(data);
}

static void	shlvl_init_when_shlvl_exists(char *shlvl, t_data *data)
{
	char	*new_number;
	int		number;

	if (ft_strlen(shlvl) == 0)
		number = 0;
	else if (check_for_non_digits(shlvl))
		number = 0;
	else
		number = ft_atoi(shlvl);
	if (number < 0)
		number = -1;
	else if (number > 998)
	{
		shlvl_warning(number + 1);
		number = 0;
	}
	new_number = ft_itoa(number + 1, data);
	update_shlvl(new_number, data);
}

void	shlvl_init(t_data *data)
{
	char	*str;

	str = ft_getenv2("SHLVL", data);
	if (!str)
		check_add("SHLVL=1", data);
	else
		shlvl_init_when_shlvl_exists(str, data);
}
