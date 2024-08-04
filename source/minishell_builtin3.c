/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtin3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:02:15 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/05 01:29:04 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

extern int	g_status;

static char	**ft_remove_env_var(char *var, char **env)
{
	int		i;
	int		j;
	int		len;
	char	**new_env;

	len = 0;
	while (env[len])
		len++;
	new_env = (char **)ft_calloc(len, sizeof(char *));
	i = 0;
	j = 0;
	while (i < (len - 1))
	{
		if ((ft_strnstr(env[j], var, ft_strlen(var)) \
			&& env[j][ft_strlen(var)] == '=' ) || \
			(ft_strnstr(env[j], var, ft_strlen(var)) \
			&& !env[j][ft_strlen(var)]))
			j++;
		new_env[i] = ft_strdup(env[j]);
		i++;
		j++;
	}
	new_env[i] = 0;
	return (new_env);
}

void	ft_unset(char *var, char ***env)
{
	char	**env2;
	char	**aux;
	int		coin;
	int		i;

	env2 = *env;
	i = 0;
	coin = 0;
	while (env2[i])
	{
		if (var && ft_strnstr(env2[i], var, ft_strlen(var)))
			if (env2[i][ft_strlen(var)] == '=' || !env2[i][ft_strlen(var)])
				coin++;
		i++;
	}
	if (coin != 0)
	{
		aux = ft_remove_env_var(var, env2);
		ft_free2dstr(env2);
		*env = aux;
	}
}

void	ft_chdir_prev(char *dir, char ***env)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = NULL;
	dir = ft_getenvval(*env, "OLDPWD=");
	old_pwd = ft_strdup(dir);
	pwd = getcwd(NULL, 0);
	ft_update_env_var("OLDPWD=", pwd, env);
	free (pwd);
	chdir(old_pwd);
	free (old_pwd);
	pwd = getcwd(NULL, 0);
	ft_update_env_var("PWD=", pwd, env);
	free (pwd);
}

void	ft_chdir(char *dir, char ***env)
{
	char	*pwd;

	if ((ft_strlen(dir) == 1 && ft_strncmp(dir, "~", 1) == 0) || !dir)
		dir = ft_getenvval(*env, "HOME");
	if ((ft_strlen(dir) == 1 && ft_strncmp(dir, "-", 1) == 0))
		ft_chdir_prev(dir, env);
	else
	{
		if (access(dir, F_OK) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(dir, 2);
			ft_putendl_fd(": No such file or directory", 2);
			g_status = 1;
		}
		pwd = getcwd(NULL, 0);
		ft_update_env_var("OLDPWD=", pwd, env);
		free (pwd);
		chdir(dir);
		pwd = getcwd(NULL, 0);
		ft_update_env_var("PWD=", pwd, env);
		free (pwd);
	}
}
