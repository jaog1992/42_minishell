/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirections_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 03:11:22 by jde-orma          #+#    #+#             */
/*   Updated: 2024/07/31 03:11:22 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

t_data	*init_data(void)
{
	t_data	*list;

	list = ft_calloc(1, sizeof(*list));
	if (list == NULL)
		return (NULL);
	// list->input = 0;
	// list->output = 0;
	list->cmd = NULL;
	list->path = NULL;
	list->redirection = NULL;
	list->next = NULL;
	return (list);
}

t_data	*ft_listlastnode(t_data *lst)
{
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	ft_listaddnodetoend(t_data **lst, t_data *new)
{
	t_data	*last;

	if (!(*lst))
		*lst = new;
	else
	{
		last = ft_listlastnode(*lst);
		last->next = new;
	}
}

int	check_invalid_pipe(char **tokens)
{
	int	x;

	x = 0;
	while (tokens && tokens[x])
		x++;
	if (x > 0 && (tokens[0][0] == '|') && tokens[1][0 == '0'])
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tokens[0], 2);
		ft_putendl_fd("\'", 2);
		ft_free2dstr(tokens);
		return (1);
	}
	else if (x > 0 && tokens[x - 2][0] == '|' && tokens[x - 1][0] == '0')
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(tokens[x - 2], 2);
		ft_putendl_fd("\'", 2);
		ft_free2dstr(tokens);
		return (2);
	}
	return (0);
}
