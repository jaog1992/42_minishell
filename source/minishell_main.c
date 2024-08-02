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

int	g_status;

// CMD_NOT_FOUND 127 -> 1111111 | 32512 -> 111111100000000
// CMD_SUCCESS 0 -> 00000000 | 512 -> 100000000
// CMD_GENERIC_ERROR 1 -> 00000001 | 512 -> 100000000
int	ft_status(char *str)
{
	if (g_status == 32512)
		g_status = CMD_NOT_FOUND;
	if (g_status == 512)
		g_status = CMD_SUCCESS;
	if (g_status == 256)
		g_status = CMD_GENERIC_ERROR;
	if (ft_strncmp(str, "$?", 2) == 0 && ft_strlen(str) == 2)
	{
		printf("%d: command not found\n", g_status);
		add_history(str);
		g_status = 127;
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

// void	ft_env(char *str, t_minishell *minishell)
// {
// 	int	i;
	
// 	i = 0;
// 	if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
// 	{
// 		while (minishell->env[i])
// 		{
// 			printf("%s\n", minishell->env[i]);
// 			i++;
// 		}
// 	}
// }


void ft_str2ddupenv(t_minishell *minishell, char *str, int len)
{
	int		i;
	char	**str2ddup;

	len = ft_str2dlen(minishell->env) + len;
	str2ddup = (char**)malloc(sizeof(char*) * (len + 1));
	str2ddup[len--] = NULL;
	i = len;
	if (ft_str2dlen(minishell->env) == len)
	{
		str2ddup[i] = ft_strdup(str);
	}
	i--;
	while(len >= 0)
	{
		str2ddup[i] = ft_strdup(minishell->env[len]);
		len--;
		i--;
	}
	ft_free2dstr(minishell->env);
	minishell->env = ft_str2ddup(str2ddup);
}

// void	ft_export(char *str, t_minishell *minishell)
// {
// 	if (ft_strncmp(str, "export ", 7) == 0 && ft_strlen(str) >= 8)
// 		ft_str2ddupenv(minishell, str + 7, 1);
// }

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

// readline returns NULL if CTRL+D as CTRL+D == EOF
// aux = ft_strjoin(aux, ft_getenvval(env, "PWD"));
// aux = ft_strjoin(ft_strjoin(aux, DEF_COLOR), " $ ");
char	*ft_get_user_input(char **env)
{
	char	*aux;
	char	*str;

	aux = ft_strjoin(ft_getenvval(env, "USER"), "@minishell $ ");
	aux = ft_strjoin(ft_strjoin(RED, aux), DEF_COLOR);
	str = readline(aux);
	if (!str)
	{
		printf("exit\n");
		if (aux)
			free(aux);
		exit(CMD_SUCCESS);
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

int	ft_datacuration(char *str, t_minishell *minishell, t_data **data)
{
	char	*aux;
	// int		i = 0;
	// t_data	*current;

	g_status = 0;
	aux = ft_variable_expansion_check(str, minishell);
	// printf("The aux string is [%s][len][%ld]\n", aux, ft_strlen(aux));
	minishell->tokens = ft_get_tokens(aux);
	if (minishell->tokens == NULL)
	{
		printf("-bash: $%s: command not found\n", str);
		return (-1);
	}
	// while (minishell->tokens[i])
	// {
	// 	printf("[token %d][%s]\n", i, minishell->tokens[i]);
	// 	i++;
	// }
	g_status = check_invalid_pipe(minishell->tokens);
	if (g_status)
	 	return (g_status);
	free(aux);
	*data = ft_redirection(minishell->tokens);
	*data = ft_commands(minishell->tokens, *data);
	// current = *data;
	g_status = check_redirection1((*data)->redirection);
	if (g_status)
	{
		ft_free2dstr(minishell->tokens);
		return (g_status);
	}
	ft_free2dstr(minishell->tokens);
	fill_cmd_path(*data, minishell->env);
	// while (current)
	// {
	// 	i = 0;
	//     if (current->tokens)
    // 	{
	// 		while (current->tokens[i] != NULL)
	// 		{
	// 			printf("[i][tokens][%d][%s]\n", i, current->tokens[i]);
	// 			i++;
	// 		}
	// 	}
	// 	else
	// 		printf("No token data available\n");
	// 	i = 0;
	//     if (current->redirection)
    // 	{
	// 		while (current->redirection[i] != NULL)
	// 		{
	// 			printf("[i][redirection][%d][%s]\n", i, current->redirection[i]);
	// 			i++;
	// 		}
	// 	}
	// 	else
	// 		printf("No redirection data available\n");
	// 	i = 0;
	//     if (current->cmd)
    // 	{
	// 		while (current->cmd[i] != NULL)
	// 		{
	// 			printf("[i][cmd][%d][%s]\n", i, current->cmd[i]);
	// 			i++;
	// 		}
	// 	}
	// 	else
	// 		printf("No redirection data available\n");
	// 	i = 0;
	//     if (current->path)
	// 		printf("[path][%s]\n", current->path);
	// 	else
	// 		printf("No path data available\n");
	// 	current = current->next;
	// }
	return (0);
}

void	ft_program(t_minishell *minishell, t_data **data)
{
	char	*str;
	int		ret;

	ft_signals();
	str = ft_get_user_input(minishell->env);
	if (str && *str != '\0')
	{
		ft_exit(str);
		if (str && *str != '\0' && ft_status(str))
		{
			add_history(str);
			ret = ft_datacuration(str, minishell, data);
			if (ret > 0)
				g_status = ret;
			if (!ret)
				ft_exec(*data, &minishell->env);
			if (*data)
				ft_freelistdata(data);
		}
		// if (minishell->tokens)
		// 	ft_free2dstr(minishell->tokens);
	}
	else if (str)
		free(str);
	// print_history();
}

t_minishell *init_minishell(char **envp)
{
	t_minishell		*minishell;

	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!minishell)
	{
		ft_print_error("Memory allocation for minishell data type failed\n");
		return (minishell);
	}
	minishell->tokens = NULL;
	minishell->env = ft_str2ddup(envp);
	return (minishell);
}

int main (int argc, char **argv, char **envp)
{
	t_data		**data;
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	g_status = 0;
	minishell = init_minishell(envp);
    data = malloc(sizeof(t_data *));  // Inicializamos data
    *data = NULL;  // Inicializamos el contenido de data a NULL
	while (TRUE)
		ft_program(minishell, data);
	//ft_free2dstr(minishell->env);
	return (0);
}