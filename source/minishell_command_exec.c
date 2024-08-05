/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_exec.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:14:25 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/26 18:14:25 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

extern int	g_status;

static int	ft_single_builtin(t_data *node, t_fd fd, char ***envp, int node_nb)
{
	if (node_nb == 1 && ft_isbuiltin(node->cmd) == 2)
	{
		ft_redirection_dup(node->redirection, &fd, node);
		ft_close(&fd.fdin, 0);
		if (ft_dup_manager(&fd, 1))
			ft_call_builtin(node->cmd, envp);
		return (0);
	}
	return (1);
}

static int	ft_exec_loop(int node_nb, t_fd *fd, char ***envp, t_data *node)
{
	int	ret;
	int	pid;

	ret = 0;
	ft_signals_in_cat();
	pid = fork();
	if (pid == 0)
	{
		while (--node_nb)
		{
			if (ft_check_cmd(node, fd, &ret, 1))
				ft_pipex(node, *envp, fd, ret);
			node = node->next;
		}
		if (ft_check_cmd(node, fd, &ret, 0))
		{
			ft_redirection_dup(node->redirection, fd, node);
			ft_child(node, *envp, fd, ret);
		}
		else
			exit(CMD_NOT_FOUND);
	}
	else
	{
		waitpid(pid, &ret, 0);
		if (WIFEXITED(ret))
		 	ret = WEXITSTATUS(ret);
	}
	return (ret);
}

void	ft_fill_here_doc(t_data *node, int node_nb)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	node->here_doc = -2;
	while (i < node_nb)
	{
		j = 0;
		if (node->redirection)
		{
			while (node->redirection[j])
			{
				if ((ft_strncmp(node->redirection[j], "<<", \
				ft_strlen(node->redirection[j]))) == 0 \
				&& ft_strlen(node->redirection[j]) == 2)
					here_doc(node->redirection[j + 1], node);
				j++;
			}
		}
		node = node->next;
		i++;
	}
}

int	ft_count_nodes(t_data *node)
{
	int		node_nb;

	node_nb = 1;
	while (node->next)
	{
		node_nb++;
		node = node->next;
	}
	return (node_nb);
}

void	ft_exec(t_data *node, char ***envp)
{
	int		node_nb;
	t_fd	fd;

	(void)envp;
	ft_init_fd(&fd);
	node_nb = ft_count_nodes(node);
	ft_fill_here_doc(node, node_nb);
	if (ft_single_builtin(node, fd, envp, node_nb))
		g_status = ft_exec_loop(node_nb, &fd, envp, node);
	ft_close_fd(&fd);
	ft_reset_fd(&fd);
}
