/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:46 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/22 18:41:46 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

void	ft_exit_error(char *str)
{
	ft_putstr_fd("exit\n\n-minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(CMD_SUCCESS);
}

int	ft_exit_arg_is_digit(char *str)
{
	int	i;
	int	len;

	i = 0;
    len = ft_strlen(str);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i] && i < len)
    {
		if (!ft_isdigit(str[i]))
			ft_exit_error(str);
		i++;
    }
	return (1);
}

int	ft_exit_arg_is_pipe(char *str)
{
	int	i;
	int	len;

	i = 0;
    len = ft_strlen(str);
	while (str[i] && i < len)
    {
		if (str[i] == '|')
			return (0);
        i++;
    }
	return (1);
}

int	ft_exit(char *str)
{
	int	num;

	num = 0;
	if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
	{
		free(str);
		printf("exit\n");
		exit(CMD_SUCCESS);
	}
	else if (ft_strncmp(str, "exit ", 5) == 0 && ft_exit_arg_is_pipe(&str[5]))
	{
		if (ft_exit_arg_is_digit(&str[5]))
		{
			printf("exit\n");
            num = atoi(&str[5]);
            free(str);
			exit(num);
		}
	}
	return (0);
}
