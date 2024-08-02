/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins_exec2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:24:44 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/02 22:24:44 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

extern int	g_status;

char	*ft_subst_var(char *var)
{
	size_t		len;

	len = 0;
	if (var)
	{
		while (var[len] && var[len] != '=')
			len++;
		if (len < ft_strlen(var))
			return (ft_substr(var, 0, len + 1));
	}
	return (0);
}

static void	ft_check_var2(char *var, char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(var, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	g_status = 1;
}

int	ft_check_var(char *var, char *cmd)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	i = 0;
	if (len == 1 && !ft_isalpha(var[0]))
	{
		ft_check_var2(var, cmd);
		return (0);
	}
	while (var[i] && i < len)
	{
		if (var[i] != '_' && ((!ft_isalpha(var[i]) && i == 0) || \
		!ft_isalnum(var[i])))
		{
			if (!(ft_builtncmp(cmd, "export", ft_strlen(cmd)) == 0 \
			&& var[i] == '=' && !var[i + 1]))
			{
				ft_check_var2(var, cmd);
				return (0);
			}
		}
		i++;
	}
	return (1);
}