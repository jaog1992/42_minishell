/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_var_expansion..c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:03:07 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/23 15:03:07 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

extern int	g_status;

int	dollar_var_len(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '?' || str[0] == '_')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*dollar_variable(char *str)
{
	char	*str2;
	int		len;
	int		i;

	len = dollar_var_len(&str[1]);
	str2 = (char *)calloc((len + 1), sizeof(char));
	ft_memset(str2, 'a', len);
	str2[len] = 0;
	i = 1;
	len = 0;
	while (str2[len])
	{
		str2[len] = str[i];
		len++;
		i++;
	}
	return (str2);
}

char	*check_var(char *var, char *one, char *second, t_minishell *mshell)
{
	if (ft_strncmp("?", var, INT64_MAX) != 0 && ft_getenvval(mshell->env, var))
	{
		second = ft_strjoin(one, ft_getenvval(mshell->env, var));
		free(one);
		return (second);
	}
	else if (ft_strncmp("?", var, INT64_MAX) == 0)
	{
		var = ft_itoa(g_status);
		second = ft_strjoin(one, var);
		free(var);
		free(one);
		return (second);
	}
	free(one);
	return (second);
}

int	ft_variable_expansion(char **str, int x, t_minishell *minishell)
{
	char	*var;
	char	*first_part;
	char	*second_part;
	char	*aux;

	var = dollar_variable((*str + x));
	first_part = ft_substr(*str, 0, x);
	second_part = NULL;
	second_part = check_var(var, first_part, second_part, minishell);
	aux = *str + x + 1 + ft_strlen(var);
	first_part = ft_strjoin(second_part, aux);
	free(second_part);
	free(*str);
	*str = first_part;
	free(var);
	return (-1);
}

char	*ft_variable_expansion_check(char *str, t_minishell *minishell)
{
	int	i;
	int	pos;

	i = 0;
	pos = -1;
	while (str[i])
	{
		if (str[i] == '"' && ft_closing_char(&str[i + 1], str[i], 0) && i > pos)
			pos = find_pos(str, i);
		else if (str[i] == '\'' && ft_closing_char(&str[i + 1], str[i], 0)
			&& i >= pos)
			i += ft_closing_char(str + i, str[i], -1);
		if (str[i] == '$' && (str[i + 1] && str[i + 1] != '"')
			&& !check_next_char(str[i + 1]))
		{
			i = ft_variable_expansion(&str, i, minishell);
			pos = -1;
		}
		i++;
	}
	return (str);
}
