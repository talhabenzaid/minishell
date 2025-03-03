/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:22:33 by tbenzaid          #+#    #+#             */
/*   Updated: 2025/03/03 00:21:54 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_home(t_env *env)
{
    while (env != NULL)
    {
        if (strncmp(env->env_var, "HOME=", 5) == 0)
            return (env->env_var + 5);
        env = env->next;
    }
    return (NULL);
}

static int update_pwd_and_oldpwd(t_data *data, char *prev_wd)
{
    char    cwd[PATH_MAX];
    char    *path;
    int     exit_code;

    exit_code = 0;
    check_add(ft_strjoin("OLDPWD=", prev_wd, data), data);
    path = getcwd(cwd, PATH_MAX);
    if (!path)
    {
        perror("cd: error retrieving current directory: getcwd: cannot access parent directories");
        if (prev_wd == NULL)
            return (1);
        path = ft_strjoin(prev_wd, "/..", data);
        check_add(ft_strjoin("PWD=", path, data), data);
        exit_code = 1;
    }
    else
        check_add(ft_strjoin("PWD=", path, data), data);
    return (exit_code);
}

static void cd_to_home(t_data *data, int *exit_status)
{
    char    *home;
    char    *prev_wd;

    prev_wd = ft_getenv2("PWD", data);
    home = get_home(data->env);
    if (!home)
    {
        printf("cd: HOME not set\n");
        *exit_status = 1;
        free(prev_wd);
        return;
    }
    if (chdir(home) == 0)
        data->exit_status = update_pwd_and_oldpwd(data, prev_wd);
    else
    {
        perror(home);
        data->exit_status = 1;
    }
}

static void cd_to_path(t_data *data, char *path, int *exit_status)
{
    char    *prev_wd;

    prev_wd = ft_getenv2("PWD", data);
    if (chdir(path) == 0)
        data->exit_status = update_pwd_and_oldpwd(data, prev_wd);
    else
    {
        perror(path);
        *exit_status = 1;
    }
}

void cd(char **str, t_data *data)
{
    data->exit_status = 0;
    
    if (str[1] != NULL && str[2] != NULL)
    {
        printf("cd: too many arguments\n");
        data->exit_status = 1;
        return;
    }
    if (str[1] == NULL)
        cd_to_home(data, &data->exit_status);
    else
        cd_to_path(data, str[1], &data->exit_status);
}

void cd_child(char **str, t_data *data)
{
    char    cwd[PATH_MAX];
    char    *path;

    if (str[1] != NULL && str[2] != NULL)
    {
        printf("cd: too many arguments\n");
        exit(1);
    }
    if (str[1] == NULL)
    {
        str[1] = get_home(data->env);
        if (str[1] == NULL)
        {
            write(2, "cd: HOME not set\n", 18);
            exit(1);
        }
    }
    if (chdir(str[1]) == 0)
    {
        path = getcwd(cwd, PATH_MAX);
        if (!path)
        {
            perror("cd: error retrieving current directory: getcwd: cannot access parent directories");
            exit(1);
        }
    }
    else
    {
        perror(str[1]);
        exit(1);
    }
    exit(0);
}
