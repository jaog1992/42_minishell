/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:34:00 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/04 11:34:00 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include "../libft/source/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

int	ft_isdupchar(char *str)
{
	if (*str == *(str + 1))
		return (2);
	return (1);
}

void	count_assign(char **aux, char **quoted_str, int *i, char a)
{
	**aux = a;
	(*quoted_str)++;
	(*aux)++;
	(*i)++;
}

int	check_assign(int flag, char a, char **quoted_str, char *c)
{
	if (a != **quoted_str && flag == 0)
	{
		flag = 1;
		*c = **quoted_str;
		(*quoted_str)++;
	}
	else if (a != **quoted_str && flag == 1)
	{
		flag = 0;
		(*quoted_str)++;
	}
	return (flag);
}

void	token_without_quotes(char *quoted_str, char *unquoted_str, char **fill)
{
	int		i;
	char	c;
	int		flag;
	char	*aux;

	i = 0;
	flag = 0;
	aux = *fill;
	while (unquoted_str[i])
	{
		flag = check_assign(flag, unquoted_str[i], &quoted_str, &c);
		if (flag == 1)
		{
			while (*quoted_str != c && unquoted_str[i])
				count_assign(&aux, &quoted_str, &i, '1');
			quoted_str++;
			flag = 0;
		}
		else if (flag == 0)
			count_assign(&aux, &quoted_str, &i, '1');
		i++;
	}
}
