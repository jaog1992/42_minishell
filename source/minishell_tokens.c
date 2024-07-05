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

int	ft_count_index(char *str)
{
	int		i;
	int		x;

	i = 0;
	x = 0;
	while (str[x])
	{
		if (str[x] == '"' || str[x] == '\'')
		{
			i += ft_next_same_char_pos(&str[x], str[x]);
			x += i + 1;
		}
		else
		{
			x++;
			i++;
		}
	}
	return (i);
}
//repasar
void	fill_array(char *source, char *dest)
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
				x = ft_next_same_char_pos(source, *source) - 1;
				check = 1;
			}
			else
			{
				x = 0;
				check = 0;
			}
			source++;
		}
		ft_2dstrncpy(&dest, &source, x);
	}
}

int	ft_isquotes(char *str)
{
	int	x;

	x = 0;
	while (str[x])
	{
		if (str[x] == '\'' || str[x] == '"')
			return (0);
		x++;
	}
	return (1);
}

char	*erase_quotes(char *str)
{
	int		x;
	char	*array;

	if (ft_isquotes(str))
		return (str);
	x = ft_count_index(str);
	if (x == 0 || x == -1)
		return (str);
	else
	{
		array = (char *)ft_calloc(x, sizeof(char));
		array[x - 1] = '\0';
		fill_array(str, array);
	}
	return (array);
}

int	check_assign(int check, char a, char **quote, char *c)
{
	if (a != **quote && check == 0)
	{
		check = 1;
		*c = **quote;
		(*quote)++;
	}
	else if (a != **quote && check == 1)
	{
		check = 0;
		(*quote)++;
	}
	return (check);
}

void	count_assign(char **aux, char **quote, int *x, char a)
{
	**aux = a;
	(*quote)++;
	(*aux)++;
	(*x)++;
}

int	ft_isdupchar(char *str)
{
	if (*str == *(str + 1))
		return (2);
	return (1);
}

int	ft_word_length(char *str)
{
	//printf("The char being aztertu is [%c] and the string is [%s]\n", *str, str);
	//printf("The strign with index is [%s]\n", &str[0]);
	if (!ft_closing_char(str, *str))
	{
		return (-1);
	}
	return (ft_next_same_char_pos(str, *str));
}

int	ft_count_words(char *str)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	//printf("ft_count_words start\n");
	while (str[x])
	{
		if ((str[x] == '"' || str[x] == '\''))
		{
			i = ft_word_length(&str[x]);
			if (i == -1)
			{
	//			printf("ft_count_words -1\n");
				return (-1);
			}
			x += i + 1;
		}
		else if (str[x] == '|' || str[x] == '<' || str[x] == '>' || str[x] == ' ')
			return (x);
		else
			x++;
	}
	//printf("ft_count_words end\n");
	return (x);
}

void	fill_array1(char *str, char **tokens)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (str[x])
	{
		if (str[x] == '|' || str[x] == '<' || str[x] == '>')
			i = ft_isdupchar(&str[x]);
		else if (str[x] != ' ')
			i = ft_count_words(&str[x]);
		if (str[x] != ' ')
		{
			*tokens = ft_substr(&str[x], 0, i);
			tokens[1] = ft_strdup(*tokens);
			ft_memset(tokens[1], '0', ft_strlen(tokens[1]));
			x += i - 1;
			tokens += 2;
		}
		x++;
	}
}

void	fill_map(char *quote, char *no, char **fill)
{
	int		check;
	char	*aux;
	char	c;
	int		x;

	x = 0;
	check = 0;
	aux = *fill;
	while (no[x])
	{
		check = check_assign(check, no[x], &quote, &c);
		if (check == 1)
		{
			while (*quote != c && no[x])
			{
				count_assign(&aux, &quote, &x, '1');
			}
			quote++;
			check = 0;
		}
		else if (check == 0)
			count_assign(&aux, &quote, &x, '1');
	}
}

void	fill_array2(char **tokens)
{
	int		x;
	char	*aux1;
	char	*aux2;
	char	*aux3;

	x = 0;
	while (tokens[x])
	{
		aux1 = erase_quotes(tokens[x]);
		if (ft_strncmp(tokens[x], aux1, __INT_MAX__) != 0)
		{
			aux2 = tokens[x];
			aux3 = tokens[x + 1];
			tokens[x] = ft_strdup(aux1);
			tokens[x + 1] = ft_strdup(aux1);
			fill_map(aux2, tokens[x], &tokens[x + 1]);
			free(aux3);
			free(aux2);
			free(aux1);
		}
		x += 2;
	}
}

//diferencia entre str && *str
// esta contando los espacios como tokens, pero debería hacerlo con las " tambien. da error
int	ft_count_tokens(char *str)
{
	int x;
	int	i;
	int	num_token;

	x = 0;
	i = 0;
	num_token = 0;
	//printf("ft_count_tokens start\n");
	while (str[x])
	{
		if (str[x] == '|' || str[x] == '<' || str[x] == '>')
			num_token++;
		else if (str[x] != ' ')
		{
			i = ft_count_words(&str[x]);
			if (i < 0)
				return (-1);
			x += i - 1;
			num_token++;
		}
		x++;
	}
	//printf("exit ft_count_tokens\n");
	return (num_token);
}

char	**ft_get_tokens(char *str, t_minishell *minishell)
{
	int		x;

	//printf("ft_get_tokens start\n");
	x = ft_count_tokens(str);
	if (x == -1)
	{
		//printf("return -1\n");
		//free (str);
		return (NULL);
	}
	//printf("[token count][%d]\n", x);
	minishell->tokens = (char **)ft_calloc(((x * 2) + 1), sizeof(char *));
	minishell->tokens[x * 2] = 0;
	if (!minishell->tokens)
		return (NULL);
	fill_array1(str, minishell->tokens);
	x = 0;
	//[borrar]
	while (minishell->tokens[x])
	{
		printf("[%d token bf fill_array 2][%s]\n", x, minishell->tokens[x]);
		x++;
	}
	fill_array2(minishell->tokens);
	return (minishell->tokens);
}
