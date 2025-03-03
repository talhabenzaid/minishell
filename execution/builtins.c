/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:13:56 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/03 02:41:57 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void echo(char **str)
{
    int i = 1;
    int option_flag = 0;
    if (str[1] == NULL)
    {
        printf("\n");
        return;
    }
    while (str[i] && str[i][0] == '-' && str[i][1] == 'n')
    {
        int j = 2;
        while (str[i][j] == 'n')
            j++;
        if (str[i][j] == '\0')
        {
            option_flag = 1;
            i++;
        }
        else
            break;
    }
    while (str[i] && strcmp(str[i], "-n") == 0)
        i++;
    while (str[i])
    {
        printf("%s", str[i]);
        if (str[i + 1])
            printf(" ");
        i++;
    }
    if (option_flag == 0)
        printf("\n");
}

void pwd()
{
    char s[PATH_MAX];
    char *path = getcwd(s, sizeof(s));
    if (!path)
        perror("getcwd");
    else
        printf("%s\n", path);
}

void env(char **str, t_data *data)
{
	t_env *head;

	head = data->env;
    if (str[1] != NULL)
    {
        printf("env: '%s' No such file or directory\n", str[1]);
        exit(127);
    }
    while (head)
    {
        if(ft_strchr(head->env_var,'='))
            printf("%s\n", head->env_var);
        head = head->next;
    }
}

void unset(char **str, t_data *data)
{
    int i = 1;
    while (str[i])
    {
        t_env *prev = NULL;
        t_env *current = data->env;
        while (current)
        {	
			if (data->env == NULL && ft_strcmp(str[i], "PATH"))
			{
				if (data->default_path)
					free(data->default_path);
				data->default_path = NULL;
			}
            else if (ft_strncmp(current->env_var, str[i], strlen(str[i])) == 0)
            {
                if (!prev)
                    data->env = current->next;
                else
                    prev->next = current->next;
                free(current->env_var);
                free(current);
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
}
