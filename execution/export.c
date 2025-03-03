/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 03:09:01 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/03 14:39:49 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void append_env(t_env *node, char *new_value, char *current_rest,t_data *data)
{
    char *exist= "";
    if (current_rest)
        exist = current_rest + 1;

    int len = ft_strlen(exist) + ft_strlen(new_value);
    char *somme = malloc(len + 1);
    if (!somme)
        free_exit(data);

    ft_strncpy(somme, exist, ft_strlen(exist));
    ft_strncpy(somme + ft_strlen(exist), new_value, ft_strlen(new_value));
    somme[len] = '\0';

    int key_len;
    if (current_rest)
        key_len = current_rest - node->env_var;
    else
        key_len = ft_strlen(node->env_var);

    int new_len = key_len + len + 2;
    char *new = malloc(new_len);
    if (!new)
    {
        free(somme);
        free_exit(data);
    }
    ft_strncpy(new, node->env_var, key_len);
    new[key_len] = '=';
    ft_strncpy(new + key_len + 1, somme, len);
    new[new_len - 1] = '\0'; 

    free(node->env_var);
    node->env_var = new;
    free(somme);
}

char	*ft_getenv2(char *str, t_data *data)
{
	t_env *current;
	if (data->env == NULL && ft_strcmp(str, "PATH") == 0)
        return(ft_strdup(data->default_path, data));
    current = data->env;
	while(current)
	{
        if (ft_strncmp(current->env_var, str, ft_strlen(str)) == 0)
            return(ft_strdup(ft_strchr(current->env_var, '='), data));
        current = current->next;
    }
	return(NULL);
}

void check_add(char *str, t_data *data)
{
    char *rest = ft_strchr(str, '=');
    int key_len;

    if (rest == NULL && ft_getenv2(str + 1, data))
        return ;
    int append = 0;
    if (rest && *(rest - 1) == '+')
    {
        append = 1;
        key_len = (rest - 1) - str;
    }
    else if (rest)
        key_len = rest - str;
    else
        key_len = ft_strlen(str);
    char *key = malloc(key_len + 1);
    if (!key)
        free_exit(data);
    ft_strncpy(key, str, key_len);
    key[key_len] = '\0';

    t_env *current = data->env;
    while (current)
    {
        char *current_rest = ft_strchr(current->env_var, '=');
        int current_key_len;
        if (current_rest)
            current_key_len = current_rest - current->env_var;
        else
            current_key_len = ft_strlen(current->env_var);

        if (ft_strncmp(current->env_var, key, current_key_len) == 0 && current_key_len == key_len)
        {
                if (append)
                    append_env(current, rest + 1, current_rest,data);
                else
                {
                    free(current->env_var);
                    current->env_var = ftt_strdup(str);
                }
            free(key);
            return;
        }
        current = current->next;
    }

        if (append)
        {
            char *new_str = malloc(ft_strlen(str) + 1);
            if (!new_str)
            {
                free(key);
                free_exit(data);
            }
            ft_strncpy(new_str, str, key_len);
            ft_strncpy(new_str + key_len, rest, ft_strlen(rest) + 1);
            add_export(new_str, data);
            free(new_str);
        }
        else
            add_export(str, data);
    free(key);
}

void export(char **str, t_data *data)
{
    int i;

    
    i = 1;
    data->exit_status = 0;
    while(str[i])
    {
        if (!is_valid(str[i]))
        {
            write(2, "bash: export: '", 15);
            write(2, str[i], ft_strlen(str[i]));
            write(2, "': not a valid identifier\n", 27);
            data->exit_status = 1;
        }
        else
        {
            check_add(str[i], data);
        }
        i++;
    }
}

void export_child(char **str, t_data *data, t_alloc **head)
{
    char **env_vars;
    int i;

    i = 0;
    data->exit_status = 0;
    if (str[1] == NULL)
    {
        env_vars = sort_export(data->env,data, head);
        while (env_vars[i])
        {
            char *key = env_vars[i];
            char *value = strchr(key, '=');
            if (value)
            {
                *value = '\0';
                value++;
				if (key[0] != '_' && key[1] != '=')
                	printf("declare -x %s=\"%s\"\n", key, value);
            } 
            else
                printf("declare -x %s\n", key);
            i++;
        }
    } 
    else 
    {
        i = 1;
        while(str[i])
        {
            if (!is_valid(str[i]))
            {
                write(2, "bash: export: '", 15);
                write(2, str[i], ft_strlen(str[i]));
                write(2, "': not a valid identifier\n", 27);
                data->exit_status = 1;
            }
            i++;
        }
	}
    ft_lstclear_garbage(head);
    exit(data->exit_status);
}
