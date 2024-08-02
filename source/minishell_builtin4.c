/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtin4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 01:35:30 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/03 01:35:30 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

extern int	g_status;

// CMD_NOT_FOUND 127 -> 1111111 | 32512 -> 111111100000000
// CMD_SUCCESS 0 -> 00000000 | 512 -> 1000000000
// CMD_GENERIC_ERROR 1 -> 00000001 | 256 -> 100000000
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

void ft_history(void)
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