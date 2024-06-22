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
#include "../libft/source/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

//extern int	g_status;
//
//int	ft_status(char *str)
//{
//	if (g_status == 32512)
//		g_status = 127;
//	if (g_status == 512)
//		g_status = 0;
//	if (g_status == 256)
//		g_status = 1;
//	if (ft_strncmp(str, "$?", 2) == 0 && ft_strlen(str) == 2)
//	{
//		printf("%d: command not found\n", g_status);
//		add_history(str);
//		free(str);
//		g_status = 127;
//		return (0);
//	}
//	return (1);
//}

void print_history(void)
{
    int i;

    i = 0;
    HIST_ENTRY **mylist = history_list ();

    if (!mylist) {
        printf("No history available.\n");
        return;
    }
    while (mylist[i])
    {
        printf("%d: %s\n", i + 1, mylist[i]->line);
        i++;
    }
}

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

// readline returns NULL if CTRL+D
char	*get_str(char **env)
{
	char	*aux;
	char	*str;

	aux = ft_strjoin(ft_getenvval(env, "USER="), "@minishell ");
    aux = ft_strjoin(ft_strjoin(GREEN, aux), DEF_COLOR);
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

//void	ft_program(char **env)
void	ft_program(char **env)
{
    char *str;
    //int ret;

    //(void)tokens;
    //(void)data;
	ft_signals();
    str = get_str(env);
	if (str && *str != '\0')
	{
		ft_exit(str);
		add_history(str);
		//if (str && *str != '\0' && ft_status(str))
		//{
		//	add_history(str);
        //    g_status++;
			//ret = general_function(str, data, *env);
	//		if (ret > 0)
	//			g_status = ret;
	//		if (!ret)
	//			ft_exec(*data, env);
	//		if (*data)
	//			ft_lstclear1(data);
	//	}
	//	if (*tokens)
	//		free_d_array(*tokens);
	//}
	if (str)
		free(str);
    print_history();
    }
}

int main (int argc, char **argv, char **envp)
{
    //extern int  g_status;
    //t_data      *data;
    //char        **tokens;
	char	    **minishel_env;

	//g_status = 0;
	(void)argc;
	(void)argv;
    //data = NULL;
    //tokens = NULL;
	minishel_env = ft_str2ddup(envp);
    while (TRUE)
		ft_program(minishel_env);
	ft_free2dstr(minishel_env);
	return (0);
}