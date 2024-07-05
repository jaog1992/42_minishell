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

int	ft_status(char *str, t_minishell minishell)
{
	if (minishell->status == 32512)
		minishell->status = 127;
	if (minishell->status == 512)
		minishell->status = 0;
	if (minishell->status == 256)
		minishell->status = 1;
	if (ft_strncmp(str, "$?", 2) == 0 && ft_strlen(str) == 2)
	{
		printf("%d: command not found\n", minishell->status);
		add_history(str);
		free(str);
		minishell->status = 127;
		return (0);
	}
	return (1);
}

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
char	*ft_get_user_input(char **env)
{
	char	*aux;
	char	*str;

	aux = ft_strjoin(ft_getenvval(env, "USER"), "@minishell ");
    aux = ft_strjoin(ft_strjoin(RED, aux), BLUE);
	aux = ft_strjoin(aux, ft_getenvval(env, "PWD"));
    aux = ft_strjoin(ft_strjoin(aux, DEF_COLOR), " $ ");
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

int	ft_general_function(char *str, t_minishell *minishell)
{
	char	*aux;
	//int		i;
	//char	**tokens;
	//int		status;

	//status = 0;
	//i = 0;
	aux = ft_variable_expansion_check(str, minishell);
    printf("The aux string is [%s][len][%ld]\n", aux, ft_strlen(aux));
	minishell->tokens = ft_get_tokens(aux, minishell);
	if (minishell->tokens == NULL)
	{
		printf("error. check input\n");
		return (0);
	}
	//while (minishell->tokens[i] && minishell->tokens != NULL)
	//{
	//	printf("[%d token][%s]\n", i, minishell->tokens[i]);
	//	i++;
	//}

	if (minishell->tokens == 0)
		return (-1);
	//status = check_pipe(minishell->tokens);
	//if (status)
	//	return (status);
	//free(aux);
	//*data = redirection(minishell->tokens);
	//*data = commands(minishell->tokens, *data);
	//status = check_redirection1((*data)->redirection);
	//if (status)
	//{
	//	ft_free2dstr(minishell->tokens);
	//	return (status);
	//}
	//ft_free2dstr(minishell->tokens);
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
    str = ft_get_user_input(minishell->env);
	if (str && *str != '\0')
	{
		ft_exit(str);
		if (str && *str != '\0' && ft_status(str))
		{
			add_history(str);
			ret = ft_general_function(str, minishell);
			if (ret > 0)
				minishell->status = ret;
	//		if (!ret)
	//			ft_exec(*data, env);
	//		if (*data)
	//			ft_lstclear1(data);
		}
		if (minishell->tokens)
			ft_free2dstr(minishell->tokens);
	//}
	if (str)
		free(str);
    print_history();
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
	minishell->status = 0;
	minishell->tokens = NULL;
	minishell->env = ft_str2ddup(envp);
    return (minishell);
}

int main (int argc, char **argv, char **envp)
{
    //t_data      *data;
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
    minishell = init_minishell(envp);
    //data = NULL;
	while (TRUE)
		ft_program(minishell);
	//ft_free2dstr(minishell->env);
	return (0);
}