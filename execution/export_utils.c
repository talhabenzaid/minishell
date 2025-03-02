/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:37:12 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/02 13:55:27 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void swap(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}


void bubble_sort(char **arr, int n)
{
    int i = 0;
    while (i < n - 1)
    {
        int j = 0;
        while (j < n - i - 1)
        {
            if (strcmp(arr[j], arr[j + 1]) > 0) 
                swap(&arr[j], &arr[j + 1]);
            j++;
        }
        i++;
    }
}

char **sort_export(t_env *head, t_data *data)
{
    char **env_vars;
    t_env *current;
    int count;
    int i;
    
    i = 0;
    count = 0;
    current = head;
    while (current)
    {
        if (current->env_var) 
            count++;
        current = current->next;
    }
    env_vars = malloc((count + 1) * sizeof(char *));
    if (!env_vars)
        free_exit(data);
    current = head;
    i = 0;
    while (current)
    {
        if (current->env_var) 
            env_vars[i++] = ftt_strdup(current->env_var);
        current = current->next;
    }
    env_vars[i] = NULL;
    bubble_sort(env_vars, count);
    current = head;
    i = 0;
    while (env_vars[i])
    {
        free(env_vars[i]);
        i++;
    }
    return (env_vars);
}


void add_export(char *str, t_data *data)
{
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        free_exit(data);
    new_node->env_var = ftt_strdup(str);
    new_node->next = NULL;
    ft_lstadd_back2(&(data->env), new_node);
}

int is_valid(const char *str)
{
    int i = 1;

    if (!str || !*str)
        return 0;
    if (!(isalpha(str[0]) || str[0] == '_'))
        return 0;
    while (str[i] && str[i] != '=')
    {
        if (isalnum(str[i]) || str[i] == '_')
        {
            i++;
            continue;
        }
        if (str[i] == '+' && str[i+1] == '=')
        {
            i++;
            break;
        }
        return (0);
    }  
    return (1);
}
