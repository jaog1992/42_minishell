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
	//printf("ft_count_index[i][index_count][%d][%d]\n", i, index_count);
	return (index_count);
}

void	ft_quotedstrncpy(char *src, char *dst)
{
	int	x;
	int j;
	int i;

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
	//	printf("ft_quoted...[i][j][%d][%d]\n", i, j);
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
	//printf("the string is %s\n", str);
	i = ft_count_index(&str[i]) + 1;
	//printf("the index is %d\n", i);
	if (i == 0 || i == -1)
		return (str);
	else
	{
		new_str = (char *)ft_calloc(i, sizeof(char));
		new_str[i - 1] = '\0';
		ft_quotedstrncpy(str, new_str);
	//	printf("[new_str][%s]\n", new_str);
	}
	return (new_str);
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

void	count_assign(char **aux, char **quoted_str, int *i, char a)
{
	**aux = a;
	(*quoted_str)++;
	(*aux)++;
	(*i)++;
}

int	ft_isdupchar(char *str)
{
	if (*str == *(str + 1))
		return (2);
	return (1);
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
			//printf("entramos con %s\n", &str[i]);
			len = ft_closing_char(&str[i], str[i], -1);
			if (len == -1)
			{
			//	printf("-1 -> [str[i], i, len][%s, %d, %d]\n", &str[i], i, len);
				return (-1);
			}
			i += len + 1;
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>' \
				 || str[i] == ' ')
			return (i);
		else
			i++;
		//printf("loop [str[i], i, len][%s, %d, %d]\n", &str[i], i, len);
	}
	return (i);
}

//Checks that '|' '<' '>' are saved alone and other none spaces are correctly allocated
//As '|' '<' '>' are also not spaces the index_step is used
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
			index_step = ft_count_chars(&str[i]);
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
	//printf("previo a erase_tokens_quotes. i es %d e index_step %d\n", i, index_step);
	//printf("fill_tokens bf erase is %s\n", tokens[0]);
	erase_tokens_quotes(tokens);
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

void	erase_tokens_quotes(char **tokens)
{
	int		i;
	char	*aux1;
	char	*aux2;
	char	*aux3;

	i = 0;
	while (tokens[i])
	{
		//printf("[%d][%s]\n", i, tokens[i]);
		aux1 = erase_quotes(tokens[i]);
		//printf("the string is [%s]\n", aux1);
		if (ft_strncmp(tokens[i], aux1, __INT_MAX__) != 0)
		{
			aux2 = tokens[i];
			aux3 = tokens[i + 1];
			tokens[i] = ft_strdup(aux1);
			tokens[i + 1] = ft_strdup(aux1);
		//	printf("tokens[%d][%d]\n", i, i + 1);
			token_without_quotes(aux2, tokens[i], &tokens[i + 1]);
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
			j = ft_count_chars(&str[i]);
			if (j < 0)
				return (-1);
			i += j - 1;
			num_token++;
		}
		i++;
	}
	return (num_token);
}

// Does a token count. If the arguments are invalid a NULL value is returned
// Else the arguments are evaluated, lexed and saved in tokens
char	**ft_get_tokens(char *str)
{
	int		i;
	char 	**tokens;

	i = ft_count_tokens(str);
	if (i == -1)
		return (NULL);
	printf("[token count][%d]\n", i);
	tokens = (char **)ft_calloc(((i * 2) + 1), sizeof(char *));
	tokens[i * 2] = 0;
	if (!tokens)
		return (NULL);
	fill_tokens(str, tokens);
	return (tokens);
}
