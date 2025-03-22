/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:47:22 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 19:54:49 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == INT_MIN)
	{
		write(fd, "-2147483648", 12);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

static int	overflow(int sign)
{
	if (sign == 1)
		return (0);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	unsigned long long			result;
	int							sign;
	unsigned long long			max;

	result = 0;
	sign = 1;
	max = 9223372036854775807;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (result > (max - (*str - '0')) / 10)
			return (overflow(sign));
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	initialize_heredoc_fds(t_data *data)
{
	int	j;

	data->fd_count = count_heredocs(data->commands);
	data->fds = ft_malloc(sizeof(int) * data->fd_count, data);
	j = 0;
	while (j < data->fd_count)
		data->fds[j++] = -1;
}
