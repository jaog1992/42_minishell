/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exec.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:47:13 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/02 16:47:13 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

int	ft_builtncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && i < (n - 1))
	{
		if (s1[i] != s2[i] && ft_toupper(s1[i]) != ft_toupper(s2[i]))
			return (1);
		i++;
	}
	return (0);
}

void	ft_call_builtin(char **cmd, char ***envp)
{
	int	i;

	i = 0;
	if (ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
		ft_export(cmd, envp);
	if (ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
		while (cmd[++i])
			if (ft_check_var(cmd[i], cmd[0]))
				ft_unset(cmd[i], envp);
	if (ft_builtncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
		ft_chdir(cmd[1], envp);
	if (ft_builtncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
		ft_env(*envp, 0);
	if (ft_builtncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
		ft_pwd();
	if (ft_builtncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		ft_echo(&cmd[1], *envp);
	if (ft_builtncmp(cmd[0], "$?", ft_strlen(cmd[0])) == 0)
		ft_status(cmd[0]);
	if (ft_builtncmp(cmd[0], "history", ft_strlen(cmd[0])) == 0)
		ft_status(cmd[0]);
}

int	ft_is_builtin(char **cmd)
{
	if (cmd)
	{
		if ((ft_builtncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 3) || (ft_builtncmp(cmd[0], "echo",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 4)
			|| (ft_builtncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 3) || (ft_builtncmp(cmd[0], "$?",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 2) ||
				(ft_builtncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 3))
			return (1);
		if ((ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 6) || (ft_strncmp(cmd[0], "unset",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 5)
			|| (ft_builtncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 2))
			return (2);
	}
	return (0);
}