/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:23:19 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/02 22:23:19 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void	ft_get_fd(char *file, int mode, t_fd *fd, t_data *node)
{
	if (mode == INPUT_REDIRECTION)
	{
		ft_close(&fd->fdin, 0);
		fd->fdin = open(file, O_RDONLY);
	}
	else if (mode == OUTPUT_REDIRECTION)
	{
		ft_close(&fd->fdout, 0);
		fd->fdout = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
	else if (mode == OUTPUT_REDIRECTION_APPEND)
	{
		ft_close(&fd->fdout, 0);
		fd->fdout = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	else if (mode == HERE_DOCUMENT)
	{
		if (fd->fdin != -2 && fd->fdin != node->here_doc)
			ft_close(&fd->fdin, 0);
		fd->fdin = node->here_doc;
	}
}

void	ft_close(int *fd, int mode)
{
	if (*fd != -2)
		close(*fd);
	if (mode == 1)
		*fd = -2;
}

void	ft_init_fd(t_fd *fd)
{
	fd->in = dup(STDIN_FILENO);
	fd->out = dup(STDOUT_FILENO);
	fd->fdin = -2;
	fd->fdout = -2;
	fd->pipe[0] = -2;
	fd->pipe[1] = -2;
	fd->here_doc = 0;
	fd->key = NULL;
}

void	ft_close_fd(t_fd *fd)
{
	ft_close(&fd->fdin, 0);
	ft_close(&fd->fdout, 0);
	ft_close(&fd->pipe[0], 0);
	ft_close(&fd->pipe[1], 0);
}

void	ft_reset_fd(t_fd *fd)
{
	dup2(STDIN_FILENO, fd->in);
	dup2(STDOUT_FILENO, fd->out);
}
