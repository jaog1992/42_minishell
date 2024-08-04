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

void	ft_freelistdata(t_data **lst)
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
