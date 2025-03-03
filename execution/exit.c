/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:30:41 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/02 23:45:28 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long ft_atol(char *str)
{
    long result = 0;
    int sign = 1;
    int i = 0;

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
        if (result > (LONG_MAX / 10) || (result == (LONG_MAX / 10) && (str[i] - '0') > (LONG_MAX % 10)))
        {
            printf("exit\n");
            printf("exit: %s: numeric argument required\n", str);
            exit(2);
        }
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return (result * sign);
}

long ft_atol2(char *str)
{
    long result = 0;
    int sign = 1;
    int i = 0;

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
        if (result > (LONG_MAX / 10) || (result == (LONG_MAX / 10) && (str[i] - '0') > (LONG_MAX % 10)))
        {
            printf("exit: %s: numeric argument required\n", str);
            exit(2);
        }
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return (result * sign);
}

int valid(char *str)
{
    int i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if(!str[i])
        return(0);
    while (str[i] != '\0')
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
void exit_child(char **str)
{

    if (str[1] != NULL)
    {
        if (str[2] != NULL)
        {
            printf("exit: too many arguments\n");
            exit(1);
        }
        else if (!valid(str[1]))
        {
            printf("exit: %s: numeric argument required\n", str[1]);
            exit(2);
        }
        else
        {
            long num = ft_atol2(str[1]);
            exit(num % 256);
        }
    }
    else
    {
        exit(0);
    }
}


void exit_program(char **str, t_data *data)
{

    if (str[1] != NULL)
    {
        if (str[2] != NULL)
        {
            printf("exit\n");
            printf("exit: too many arguments\n");
            data->exit_status = 1;
            return;
        }
        else if (!valid(str[1]))
        {
            printf("exit\n");
            printf("exit: %s: numeric argument required\n", str[1]);
            exit(2);
        }
        else
        {
            long num = ft_atol(str[1]);
            printf("exit\n");
            exit(num % 256);
        }
    }
    else
    {
        printf("exit\n");
        exit(0);
    }
}
