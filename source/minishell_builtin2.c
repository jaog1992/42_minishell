/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtin2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 09:34:32 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/05 01:29:44 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

static char	**ft_add_var(char *var, char *value, char **env)
{
	char	**new_env;
	int		len;
	int		i;

	len = 0;
	while (env[len])
		len++;
	new_env = (char **)ft_calloc(len + 2, sizeof(char *));
	i = 0;
	while (i < len)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	if (value)
		new_env[i] = ft_strjoin(var, value);
	else
		new_env[i] = ft_substr(var, 0, ft_strlen(var));
	new_env[++i] = 0;
	return (new_env);
}

static int	ft_edit_var2(char *var, char *value, char ***env, int i)
{
	char	**env2;

	env2 = *env;
	if (var && (ft_strlen(var) - 1) == ft_strlen(env2[i])
		&& var[ft_strlen(var) - 1] == '=')
	{
		if (ft_strnstr(var, env2[i], ft_strlen(env2[i])))
		{
			free(env2[i]);
			env2[i] = ft_strjoin(var, value);
			return (1);
		}
	}
	return (0);
}

static int	ft_edit_var(char *var, char *value, char ***env, int i)
{
	char	**env2;

	env2 = *env;
	if (var && ft_strnstr(env2[i], var, ft_strlen(var)))
	{
		if (value)
		{
			free(env2[i]);
			env2[i] = ft_strjoin(var, value);
			return (1);
		}
		else if (!env2[i][ft_strlen(var)] || env2[i][ft_strlen(var)] == '=')
		{
			free(env2[i]);
			env2[i] = ft_substr(var, 0, ft_strlen(var));
			return (1);
		}
	}
	return (0);
}

void	ft_update_env_var(char *var, char *value, char ***env)
{
	char	**env2;
	char	**aux;
	int		coin;
	int		i;

	env2 = *env;
	i = -1;
	coin = 0;
	while (env2[++i])
	{
		coin += ft_edit_var(var, value, env, i);
		coin += ft_edit_var2(var, value, env, i);
	}
	if (var && coin == 0)
	{
		aux = ft_add_var(var, value, env2);
		ft_free2dstr(env2);
		*env = aux;
	}
}

void	ft_export(char **cmd, char ***env)
{
	int		i;
	char	*var;

	i = 1;
	var = NULL;
	if (!cmd[i])
		ft_env(*env, ENV_EXPORT_FORMAT);
	while (cmd[i])
	{
		var = ft_subst_var(cmd[i]);
		if (var)
		{
			if (ft_check_var(var, cmd[0]))
				ft_update_env_var(var, (ft_strchr(cmd[i], '=') + 1), env);
			free(var);
			var = NULL;
		}
		else
		{
			if (ft_check_var(cmd[i], cmd[0]))
				ft_update_env_var(cmd[i], NULL, env);
		}
		i++;
	}
}
