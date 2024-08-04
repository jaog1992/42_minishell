/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_var_expansion_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:03:07 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/23 15:03:07 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

int	check_next_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '?')
		return (0);
	return (1);
}

int	ft_closing_char(char *str, char c, int ret)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (ret);
}

int	find_pos(char *str, int x)
{
	int	i;

	i = x + 1;
	while (str[i])
	{
		if (str[i] == str[x])
			return (i);
		i++;
	}
	return (-1);
}
