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
			token_without_quotes(aux2, tokens[i], &tokens[i + 1]);
			free(aux3);
			free(aux2);
			free(aux1);
		}
		i += 2;
	}
}

// Checks that '|' '<' '>' are saved alone and other none spaces are correctly 
// allocated
// As '|' '<' '>' are also not spaces the index_step is used
void	fill_tokens(char *str, char **tokens)
{
	int	i;
	int	j;
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
	erase_tokens_quotes(tokens);
}

int	ft_count_tokens(char *str)
{
	int	i;
	int	j;
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
// printf("[token count][%d]\n", i);
char	**ft_get_tokens(char *str)
{
	int		i;
	char	**tokens;

	i = ft_count_tokens(str);
	if (i == -1)
		return (NULL);
	tokens = (char **)ft_calloc(((i * 2) + 1), sizeof(char *));
	tokens[i * 2] = 0;
	if (!tokens)
		return (NULL);
	fill_tokens(str, tokens);
	return (tokens);
}
