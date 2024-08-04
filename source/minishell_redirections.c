/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 03:10:01 by jde-orma          #+#    #+#             */
/*   Updated: 2024/07/31 03:10:01 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

int	count_redirection_char_len(char **tokens, int i)
{
	if ((tokens[i][0] == '<' || tokens[i][0] == '>') && tokens[i + 1][0] == '0')
	{
		if (tokens[i + 2] && tokens[i + 2][0] != '|')
			return (2);
		else
			return (1);
	}
	return (0);
}

int	malloc_redirection(char **tokens, t_data **node)
{
	int	i;
	int	len;
	int	index;

	i = 0;
	len = 0;
	index = 0;
	while (tokens[i])
	{
		if (tokens[i] && tokens[i][0] == '|' && tokens[i + 1][0] == '0')
			break ;
		len += count_redirection_char_len(tokens, i);
		i += 2;
	}
	if (tokens[i] && tokens[i][0] == '|' && tokens[i + 1][0] == '0')
		index = i;
	if (len > 0)
	{
		(*node)->redirection = (char **)malloc((len + 1) * sizeof(char *));
		(*node)->redirection[len] = 0;
	}
	return (index);
}

// Malloc redirection checks the number of redirections in tokens
// Each block is surrounded either by pipes or stdin/stdout
int	fill_redirection(char **tokens, t_data *node)
{
	int	i;
	int	j;
	int	pipe_index;

	pipe_index = malloc_redirection(tokens, &node);
	i = 0;
	j = 0;
	while (tokens[i])
	{
		if (tokens[i] && tokens[i][0] == '|' && tokens[i + 1][0] == '0')
			break ;
		if ((tokens[i][0] == '<' || tokens[i][0] == '>') && tokens[i
			+ 1][0] == '0')
		{
			node->redirection[j] = ft_strdup(tokens[i]);
			if (tokens[i + 2] && tokens[i + 2][0] != '|')
				node->redirection[++j] = ft_strdup(tokens[i + 2]);
			j++;
		}
		i += 2;
	}
	return (pipe_index);
}

t_data	*ft_redirection(char **tokens)
{
	int		i;
	int		pipe_index;
	t_data	*aux;
	t_data	*nodes;

	i = 0;
	pipe_index = -1;
	nodes = init_data();
	aux = nodes;
	while (tokens && tokens[i])
	{
		nodes = ft_listlastnode(nodes);
		pipe_index = fill_redirection(&tokens[i], nodes);
		if (pipe_index == 0)
			break ;
		else
			i += pipe_index;
		ft_listaddnodetoend(&aux, init_data());
		i += 2;
	}
	return (aux);
}
