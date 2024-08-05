/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_double_redirection.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 21:49:00 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/02 21:49:00 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"
#include <sys/wait.h>

extern int	g_status;

char	*add_new_line_char(char *aux, char *str)
{
	char	*temp;

	temp = ft_strjoin(aux, "\n");
	free(aux);
	aux = temp;
	temp = ft_strjoin(aux, str);
	free(aux);
	aux = temp;
	return (aux);
}

void	double_redirection(char *key, char *aux)
{
	char	*str;

	aux = 0;
	while (TRUE)
	{
		ft_signals_in_cat();
		aux = ft_strjoin(ft_strjoin(GREEN, "heredoc> "), DEF_COLOR);
		str = readline(aux);
		if (!str)
		{
			if (aux)
				free(aux);
			break ;
		}
		if (!ft_strncmp(str, key, __INT_MAX__))
			break ;
		if (!aux)
			aux = ft_strdup(str);
		else
			aux = add_new_line_char(aux, str);
		free(str);
	}
	if (str)
		free(str);
	return ;
}

void	print2buffer(char *str, int fd)
{
	ft_putendl_fd(str, fd);
	free(str);
}

//pipe(int fd[2]) makes an unidirectional comms channel between 2 processes
// if for the child and else for the father. The wait command wait the child
// to end execution
void	here_doc(char *key, t_data *node)
{
	int		fd1[2];
	pid_t	pid;
	char	*str;

	pipe(fd1);
	pid = fork();
	str = 0;
	if (pid == 0)
	{
		close(fd1[0]);
		double_redirection(key, str);
		if (str)
			print2buffer(str, fd1[1]);
		close(fd1[1]);
		exit(CMD_SUCCESS);
	}
	else
	{
		close(fd1[1]);
		wait(NULL);
		if (node->here_doc > 1)
			close(node->here_doc);
		node->here_doc = dup(fd1[0]);
		close(fd1[0]);
	}
}
