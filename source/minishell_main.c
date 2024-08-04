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

int	ft_lexer(char *str, t_minishell *minishell, t_data **data)
{
	char	*aux;

	g_status = 0;
	add_history(str);
	aux = ft_variable_expansion_check(str, minishell);
	minishell->tokens = ft_get_tokens(aux);
	if (minishell->tokens == NULL)
	{
		printf("-bash: $%s: command not found\n", str);
		return (-1);
	}
	g_status = check_invalid_pipe(minishell->tokens);
	if (g_status)
		return (g_status);
	free(aux);
	*data = ft_redirection(minishell->tokens);
	*data = ft_commands(minishell->tokens, *data);
	g_status = check_invalid_redirection((*data)->redirection);
	ft_free2dstr(minishell->tokens);
	if (g_status)
		return (g_status);
	fill_cmd_path(*data, minishell->env);
	return (0);
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

// void	ft_print_data(t_data **data)
// {
// 	int		i;
// 	t_data	*current;

// 	current = *data;
// 	while (current)
// 	{
// 		printf("[input][%d]\n", current->input);
// 		printf("[output][%d]\n", current->output);
// 		printf("[here_doc][%d]\n", current->here_doc);
// 		if (current->path)
// 			printf("[path][%s]\n", current->path);
// 		else
// 			printf("No path data available\n");
// 		i = 0;
// 		if (current->cmd)
// 		{
// 			while (current->cmd[i] != NULL)
// 			{
// 				printf("[i][cmd][%d][%s]\n", i, current->cmd[i]);
// 				i++;
// 			}
// 		}
// 		else
// 			printf("No command data available\n");
// 		i = 0;
// 		if (current->tokens)
// 		{
// 			while (current->tokens[i] != NULL)
// 			{
// 				printf("[i][tokens][%d][%s]\n", i, current->tokens[i]);
// 				i++;
// 			}
// 		}
// 		else
// 			printf("No token data available\n");
// 		i = 0;
// 		if (current->redirection)
// 		{
// 			while (current->redirection[i] != NULL)
// 			{
// 				printf("[i][redir][%d][%s]\n", i, current->redirection[i]);
// 				i++;
// 			}
// 		}
// 		else
// 			printf("No redirection data available\n");
// 		current = current->next;
// 	}
// }

void	ft_program(t_minishell *minishell, t_data **data)
{
	int		ret;
	char	*str;

	ft_signals();
	str = ft_get_user_input(minishell->env);
	if (str && *str != '\0')
	{
		ft_exit(str);
		if (str && *str != '\0' && ft_status(str))
		{
			ret = ft_lexer(str, minishell, data);
			if (ret > 0)
				g_status = ret;
			if (!ret)
				ft_exec(*data, &minishell->env);
			if (*data)
				ft_freelistdata(data);
		}
	}
	else if (str)
		free(str);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*minishell;

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

int	main(int argc, char **argv, char **envp)
{
	t_data		**data;
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	g_status = 0;
	minishell = init_minishell(envp);
	data = malloc(sizeof(t_data *));
	*data = NULL;
	while (TRUE)
		ft_program(minishell, data);
	ft_free2dstr(minishell->env);
	return (0);
}
