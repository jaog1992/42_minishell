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
#include <readline/history.h>
#include <readline/readline.h>

int	dollar_var_len(char *str)
{
	int	x;

	x = 0;
	if (str[0] == '?' || str[0] == '_')
		return (1);
	while (str[x] && ((str[x] >= '0' && str[x] <= '9') 
            || (str[x] >= 'A' && str[x] <= 'Z')	|| (str[x] >= 'a' && str[x] <= 'z')
            || str[x] == '_'))
		x++;
	return (x);
}

int	count_char_index(char *str, char a)
{
	int	x;

	x = 1;
	while (str[x])
	{
		if (str[x] == a)
			return (x);
		x++;
	}
	return (-1);
}

char	*dollar_variable(char *str)
{
	char	*array;
	int		len;
	int		x;

	len = dollar_var_len(&str[1]);
	array = (char *)calloc((len + 1), sizeof(char));
	ft_memset(array, 'a', len);
	array[len] = 0;
	len = 0;
	x = 1;
	while (array[len])
	{
		array[len] = str[x];
		len++;
		x++;
	}
	return (array);
}

int	find_pos(char *str, int x)
{
	int	i;

	i = x + 1;
	while (str[i])
	{
		if (str[i] == str[x])
			return (i);
		i++;
	}
	return (-1);
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
		var = ft_itoa(mshell->g_status);
		second = ft_strjoin(one, var);
		free(var);
		free(one);
		return (second);
	}
	free(one);
	return (second);
}

int	ft_variable_expansion(char **str, int x, t_minishell *mshell)
{
	char	*var;
	char	*first_part;
	char	*second_part;
	char	*aux;

	var = dollar_variable((*str + x));
	first_part = ft_substr(*str, 0, x);
	second_part = NULL;
	second_part = check_var(var, first_part, second_part, mshell);
	aux = *str + x + 1 + ft_strlen(var);
	first_part = ft_strjoin(second_part, aux);
	free(second_part);
	free(*str);
	*str = first_part;
	free(var);
	return (-1);
}

int	closing_char_exists(char *str, char c)
{
	int i;
    
    i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	check_next_char(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a'
			&& c <= 'z') || c == '_' || c == '?')
		return (0);
	return (1);
}

char	*ft_variable_expansion_check(char *str, t_minishell *minishell)
{
	int	x;
	int	i;

	i = -1;
	x = 0;
	while (str[x])
	{
		if (str[x] == '"' && closing_char_exists(&str[x + 1], str[x]) && x > i)
			i = find_pos(str, x);
		else if (str[x] == '\'' && closing_char_exists(&str[x + 1], str[x])
			&& x >= i)
			x += count_char_index(str + x, str[x]);
		if (str[x] == '$' && (str[x + 1] && str[x + 1] != '"')
			&& !check_next_char(str[x + 1]))
		{
			x = ft_variable_expansion(&str, x, minishell);
			i = -1;
		}
		x++;
	}
	return (str);
}