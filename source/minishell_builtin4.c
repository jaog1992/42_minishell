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

int	ft_status(char *str)
{
	if (g_status == CMD_NOT_FOUND_SHIFTED)
		g_status = CMD_NOT_FOUND;
	if (g_status == CMD_SUCCESS_SHIFTED)
		g_status = CMD_SUCCESS;
	if (g_status == CMD_GENERIC_ERROR_SHIFTED)
		g_status = CMD_GENERIC_ERROR;
	if (ft_strncmp(str, "$?", 2) == 0 && ft_strlen(str) == 2)
	{
		printf("%d: command not found\n", g_status);
		add_history(str);
		g_status = CMD_NOT_FOUND;
		return (0);
	}
	return (1);
}

void	ft_str2ddupenv(t_minishell *minishell, char *str, int len)
{
	int		i;
	char	**str2ddup;

	len = ft_str2dlen(minishell->env) + len;
	str2ddup = (char **)malloc(sizeof (char *) * (len + 1));
	str2ddup[len--] = NULL;
	i = len;
	if (ft_str2dlen(minishell->env) == len)
	{
		str2ddup[i] = ft_strdup(str);
	}
	i--;
	while (len >= 0)
	{
		str2ddup[i] = ft_strdup(minishell->env[len]);
		len--;
		i--;
	}
	ft_free2dstr(minishell->env);
	minishell->env = ft_str2ddup(str2ddup);
}

char	*ft_getenvval(char **env, char *str)
{
	int		i;
	char	*envval;
	char	*aux;

	i = 0;
	envval = NULL;
	aux = ft_strjoin(str, "=");
	while (env[i])
	{
		if (ft_strnstr(env[i], aux, ft_strlen(aux)))
		{
			envval = ft_strchr(env[i], '=');
			free(aux);
			return (++envval);
		}
		i++;
	}
	free(aux);
	return ("");
}

void	ft_history(void)
{
	int			i;
	HIST_ENTRY	**mylist;

	i = 0;
	mylist = history_list ();
	if (!mylist)
	{
		printf("No history available.\n");
		return ;
	}
	while (mylist[i])
	{
		printf("%d: %s\n", i + 1, mylist[i]->line);
		i++;
	}
}
