/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 09:09:14 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/22 09:09:14 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

extern int	g_status;

int	check_invalid_pipe(char **tokens)
{
	int	len;

	len = 0;
	while (tokens && tokens[len])
		len++;
	if (len > 0 && (tokens[0][0] == '|') && tokens[1][0] == '0')
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tokens[0], 2);
		ft_putendl_fd("\'", 2);
		ft_free2dstr(tokens);
		return (1);
	}
	else if (len > 0 && tokens[len - 2][0] == '|' && tokens[len - 1][0] == '0')
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tokens[len - 2], 2);
		ft_putendl_fd("\'", 2);
		ft_free2dstr(tokens);
		return (2);
	}
	return (0);
}

void	ft_lstclear1(t_data **lst)
{
	t_data	*aux;

	while (*lst)
	{
		aux = (*lst)->next;
		if ((*lst)->cmd != NULL)
		{
			if (&(*lst)->cmd[0] != &(*lst)->path)
				ft_free2dstr((*lst)->cmd);
		}
		if ((*lst)->path != NULL)
			free((*lst)->path);
		if ((*lst)->redirection != NULL)
			ft_free2dstr((*lst)->redirection);
		free(*lst);
		*lst = aux;
	}
}

int	check_redirection1(char **ptr)
{
	int	i;

	i = 0;
	while (ptr != NULL && ptr[i])
	{
		if ((ptr[i][0] == '<' || ptr[i][0] == '>') &&
			ptr[i + 1] && (ptr[i + 1][0] == '<' || ptr[i + 1][0] == '>'))
		{
			i++;
			while (ptr[i] && (ptr[i][0] == '<' || ptr[i][0] == '>'))
				i++;
			printf("bash: syntax error near unexpected token `");
			printf("%s\'", ptr[i - 2]);
			return (1);
		}
		else if ((ptr[i][0] == '<' || ptr[i][0] == '>') && ptr[i + 1] == NULL)
		{
			printf("bash: syntax error near unexpected token 'newline'\n");
			return (2);
		}
		i += 2;
	}
	return (0);
}