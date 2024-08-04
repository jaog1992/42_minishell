/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens_utils1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:27:00 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/04 11:27:00 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include "../libft/source/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

int	ft_count_index(char *str)
{
	int		i;
	int		index_count;
	int		closing_char_pos;
	char	*aux;

	i = 0;
	index_count = 0;
	closing_char_pos = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			aux = (str + i);
			closing_char_pos = ft_closing_char(aux, str[i], -1);
			index_count += closing_char_pos;
			i += closing_char_pos + 1;
		}
		else
		{
			i++;
			index_count++;
		}
	}
	return (index_count);
}

void	ft_quotedstrncpy(char *src, char *dst)
{
	int	x;
	int	j;
	int	i;

	i = 0;
	j = 0;
	x = 0;
	while (src[i])
	{
		if ((src[i] == '\'' || src[i] == '"'))
		{
			x = ft_closing_char(&src[i], src[i], -1) - 1;
			ft_strncpy(&dst[j], &src[i + 1], x);
			i += x + 1;
			j += x;
		}
		else
			ft_strncpy(&dst[j++], &src[i], 1);
		i++;
	}
}

int	ft_isquotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	*erase_quotes(char *str)
{
	int		i;
	char	*new_str;

	if (!ft_isquotes(str))
		return (str);
	i = 0;
	i = ft_count_index(&str[i]) + 1;
	if (i == 0 || i == -1)
		return (str);
	else
	{
		new_str = (char *)ft_calloc(i, sizeof(char));
		new_str[i - 1] = '\0';
		ft_quotedstrncpy(str, new_str);
	}
	return (new_str);
}

int	ft_count_chars(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\''))
		{
			len = ft_closing_char(&str[i], str[i], -1);
			if (len == -1)
				return (-1);
			i += len + 1;
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>' \
				|| str[i] == ' ')
			return (i);
		else
			i++;
	}
	return (i);
}
