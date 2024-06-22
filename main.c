/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 20:00:32 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/22 09:07:43 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenvval(char** env, char *str)
{
    int i;
    int j;
    int flag;
    char *str2;

    i = 0;
    j = 0;
    flag = 0;
    if (!env || !str)
        return NULL;
    while(env[i])
    {
        while (str[j])
        {
            if (str[j] != env[i][j])
                flag++;
            j++;
        }
        if (flag == 0)
            break;
        j = 0;
        flag = 0;
        i++;
    }
    if (!env[i])
        return NULL;
    str2 = ft_strdup(env[i] + ft_strlen(str));
    return (str2);
}

char	*get_str(char **env)
{
	char	*aux;
	char	*user;
	char	*shell;
	char	*str;

    user = ft_strjoin(ft_getenvval(env, "USER="), "@");
    shell = ft_strjoin(ft_getenvval(env, "_=./"), " $");
    aux = ft_strjoin(user, shell);
    free(user);
    free(shell);
	str = readline(aux);
	if (!str)
	{
		printf("exit\n");
		if (aux)
			free(aux);
		exit(0);
	}
	free(aux);
	if (str == NULL || *str == 0)
	{
		if (*str == 0)
			free(str);
		return (NULL);
	}
	aux = ft_strtrim(str, " ");
	free(str);
	return (aux);
}

void	ft_program(char **env2)
{
	int	 ret;
    char *str;

	ret = 0;
	ft_signals();
	str = get_str(env2);
    printf("The string is %s\n", str);
	//if (str && *str != '\0')
	//{
	//	ft_exit(str);
	//	if (str && *str != '\0' && ft_status(str))
	//	{
	//		add_history(str);
	//		ret = general_function(str, data, *env2);
	//		if (ret > 0)
	//			g_status = ret;
	//		if (!ret)
	//			ft_exec(*data, env2);
	//		if (*data)
	//			ft_lstclear1(data);
	//	}
	//	if (*tokens)
	//		free_d_array(*tokens);
	//}
	//else if (str)
	//	free(str);
}

int main (int argc, char **argv, char **envp)
{
    int i;
    //char *user;
    //char *shell;
    //char *mini;
    char **mini_env;

    i = 0;
    (void)argc;
    (void)argv;
    mini_env = ft_dblstrdup(envp);
    //ft_program(mini_env);
    //user = ft_strjoin(ft_getenvval(mini_env, "USER="), "@");
    //shell = ft_strjoin(ft_getenvval(mini_env, "_=./"), " $");
    //mini = ft_strjoin(user, shell);
    //printf("\033[0;92m%s\033[0;39m\n", mini);
    //free(user);
    //free(shell);
    ft_free(mini_env);
    return (0);
}