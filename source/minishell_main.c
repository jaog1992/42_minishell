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
    if (!env[i] || j == 0)
        return NULL;
    str2 = ft_strdup(env[i] + ft_strlen(str) + 1);
    return (str2);
}

// readline returns NULL if CTRL+D
char	*get_str(char **env)
{
	char	*aux;
	char	*str;

	aux = ft_strjoin(ft_getenvval(env, "USER"), "@minishell ");
    aux = ft_strjoin(ft_strjoin(RED, aux), BLUE);
	aux = ft_strjoin(aux, ft_getenvval(env, "PWD"));
    aux = ft_strjoin(aux, DEF_COLOR);
    aux = ft_strjoin(aux, " $ ");
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

int ft_status(char *str)
{
    if(!str)
        return (0);
    return (1);
}

int	general_function(char *str, t_minishell *minishell)
{
	char	*aux;
	//char	**tokens;
	//int		status;

	//status = 0;
	aux = ft_variable_expansion_check(str, minishell);
    printf("The aux string is [%s]\n", aux);
	minishell->tokens = fill_tokens(aux, ft_strlen(aux));
	//if (minishell->tokens == 0)
	//	return (-1);
	//status = check_pipe(minishell->tokens);
	//if (status)
	//	return (status);
	free(aux);
	//*data = redirection(minishell->tokens);
	//*data = commands(minishell->tokens, *data);
	//status = check_redirection1((*data)->redirection);
	//if (status)
	//{
	//	free_d_array(minishell->tokens);
	//	return (status);
	//}
	//free_d_array(minishell->tokens);
	//fill_cmd_path(*data, env);
	return (0);
}


//void	ft_program(char **env)
void	ft_program(t_minishell *minishell)
{
    char *str;
    int ret;

    //(void)tokens;
    //(void)data;
	ft_signals();
    str = get_str(minishell->env);
	if (str && *str != '\0')
	{
		ft_exit(str);
		if (str && *str != '\0' && ft_status(str))
		{
			add_history(str);
			ret = general_function(str, minishell);
			if (ret > 0)
				minishell->g_status = ret;
	//		if (!ret)
	//			ft_exec(*data, env);
	//		if (*data)
	//			ft_lstclear1(data);
		}
	//	if (*tokens)
	//		free_d_array(*tokens);
	//}
	//if (str)
	//	free(str);
    //print_history();
    }
}

t_minishell *init_minishell(char **envp)
{
	t_minishell	    *minishell;

	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!minishell)
	{
		ft_print_error("Memory allocation for minishell data type failed\n");
		return (minishell);
	}
	minishell->g_status = 0;
	minishell->tokens = NULL;
	minishell->env = ft_str2ddup(envp);
    return (minishell);
}

int main (int argc, char **argv, char **envp)
{
    //t_data      *data;
	t_minishell	    *minishell;

	(void)argc;
	(void)argv;
    minishell = init_minishell(envp);
    //data = NULL;
    while (TRUE)
		ft_program(minishell);
	//ft_free2dstr(minishell->env);
	return (0);
}