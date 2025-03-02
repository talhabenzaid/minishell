/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:22:33 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/02 13:22:41 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_home(t_env *env)
{
    while (env != NULL)
    {
        if (strncmp(env->env_var, "HOME=", 5) == 0)
        {
            return env->env_var + 5;
        }
        env = env->next;
    }
    return (NULL);
}
void path(char *str,t_data *data)
{
    char s[PATH_MAX];
    char *path = getcwd(s, sizeof(s));
    if (!path)
        perror("getcwd");
    check_add(ft_strjoin(str, path, data),data);
}
void cd(char **str, t_data *data)
{
    if(str[1] != NULL && str[2] != NULL)
    {
        printf("cd: too many arguments\n");
		data->exit_status = 1;
        return;
    }
    if (str[1] == NULL)
    {   
        str[1] = get_home(data->env);;
        if (str[1] == NULL)
        {
            printf("cd: HOME not set\n");
			data->exit_status = 1;
            return;
        }
    }
    path("OLDPWD=",data);
    if (chdir(str[1]))
    {
        perror(str[1]);
		data->exit_status = 1;
        return;
    }
    path("PWD=",data);
}