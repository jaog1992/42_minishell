/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 01:30:58 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/24 01:30:58 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include "../libft/source/libft.h"
#include <readline/history.h>
#include <readline/readline.h>

extern int	g_status;

int	ft_count_index(char *str)
{
	// int		i;
	// int		j;

	// i = 0;
	// j = 0;
	// while (str[i])
	// {
	// 	if (str[i] == '"' || str[i] == '\'')
	// 	{
	// 		j += ft_next_same_char_pos(&str[i], str[i]);
	// 		i += j + 1;
	// 	}
	// 	else
	// 	{
	// 		i++;
	// 		j++;
	// 	}
	// }
	// printf("the value of count index is [%d]\n", j);
	// return (j);
	int		x;
	int		i;
	int		q;
	char	*aux;

	q = 0;
	x = 0;
	i = 0;
	while (str[x])
	{
		if (str[x] == '"' || str[x] == '\'')
		{
			aux = (str + x);
			q = ft_closing_char(aux, str[x], -1);
			i += q;
			x += q + 1;
		}
		else
		{
			x++;
			i++;
		}
	}
	return (i);
}

void	replace_string(char *source, char *dest)
{
	int	x;
	int	check;

	check = 0;
	while (*source)
	{
		x = 1;
		if ((*source == '\'' || *source == '"'))
		{
			if (check == 0)
			{
				x = ft_closing_char(source, *source, -1) - 1;
				check = 1;
			}
			else
			{
				x = 0;
				check = 0;
			}
			source++;
		}
		if (check == 1)
		{
			ft_strncpy(dest, source, x);
			return ;
		}
	}
}

int	ft_isquotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (0);
		i++;
	}
	return (1);
}

char	*erase_quotes(char *str)
{
	int		i;
	char	*new_str;

	if (ft_isquotes(str))
		return (str);
	i = ft_count_index(str);
	if (i == 0 || i == -1)
		return (str);
	else
	{
		new_str = (char *)ft_calloc(i, sizeof(char));
		new_str[i - 1] = '\0';
		replace_string(str, new_str);
	}
	return (new_str);
}

int	check_assign(int check, char a, char **quoted_str, char *c)
{
	if (a != **quoted_str && check == 0)
	{
		check = 1;
		*c = **quoted_str;
		(*quoted_str)++;
	}
	else if (a != **quoted_str && check == 1)
	{
		check = 0;
		(*quoted_str)++;
	}
	return (check);
}

void	count_assign(char **aux, char **quoted_str, int *x, char a)
{
	**aux = a;
	(*quoted_str)++;
	(*aux)++;
	(*x)++;
}

int	ft_isdupchar(char *str)
{
	if (*str == *(str + 1))
		return (2);
	return (1);
}

int	ft_count_words(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\''))
		{
			len = ft_closing_char(str, str[i], -1);
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

void	fill_tokens(char *str, char **tokens)
{
	int	i;
	int j;
	int	index_step;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
			index_step = ft_isdupchar(&str[i]);
		else if (str[i] != ' ')
			index_step = ft_count_words(&str[i]);
		if (str[i] != ' ')
		{
			tokens[j] = ft_substr(&str[i], 0, index_step);
			tokens[j + 1] = ft_strdup(*tokens);
			ft_memset(tokens[j + 1], '0', ft_strlen(tokens[j + 1]));
			i += index_step - 1;
			j += 2;
		}
		i++;
	}
	erase_tokens_quotes(tokens);
}

//REVISAR
void	fill_map(char *quoted_str, char *unquoted_str, char **fill)
{
	int		check;
	char	*aux;
	char	c;
	int		x;

	x = 0;
	check = 0;
	aux = *fill;
	while (unquoted_str[x])
	{
		check = check_assign(check, unquoted_str[x], &quoted_str, &c);
		if (check == 1)
		{
			while (*quoted_str != c && unquoted_str[x])
			{
				count_assign(&aux, &quoted_str, &x, '1');
			}
			quoted_str++;
			check = 0;
		}
		else if (check == 0)
			count_assign(&aux, &quoted_str, &x, '1');
	}
}

void	erase_tokens_quotes(char **tokens)
{
	int		i;
	char	*aux1;
	char	*aux2;
	char	*aux3;

	i = 0;
	while (tokens[i])
	{
		aux1 = erase_quotes(tokens[i]);
		if (ft_strncmp(tokens[i], aux1, __INT_MAX__) != 0)
		{
			aux2 = tokens[i];
			aux3 = tokens[i + 1];
			tokens[i] = ft_strdup(aux1);
			tokens[i + 1] = ft_strdup(aux1);
			printf("tokens[%d][%d]\n", i, i + 1);
			fill_map(aux2, tokens[i], &tokens[i + 1]);
			free(aux3);
			free(aux2);
			free(aux1);
		}
		i += 2;
	}
}

int	ft_count_tokens(char *str)
{
	int	i;
	int j;
	int	num_token;

	i = 0;
	j = 0;
	num_token = 0;
	while (str[i])
	{
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
			num_token++;
		else if (str[i] != ' ')
		{
			j = ft_count_words(&str[i]);
			if (j < 0)
				return (-1);
			i += j - 1;
			num_token++;
		}
		i++;
	}
	return (num_token);
}

char	**ft_get_tokens(char *str, t_minishell *minishell)
{
	int		i;

	i = ft_count_tokens(str);
	if (i == -1)
		return (NULL);
	printf("[token count][%d]\n", i);
	minishell->tokens = (char **)ft_calloc(((i * 2) + 1), sizeof(char *));
	minishell->tokens[i * 2] = 0;
	if (!minishell->tokens)
		return (NULL);
	fill_tokens(str, minishell->tokens);
	i = 0;
	while (minishell->tokens[i])
	{
		printf("[%d token bf fill_array 2][%s]\n", i, minishell->tokens[i]);
		i++;
	}
	return (minishell->tokens);
}
