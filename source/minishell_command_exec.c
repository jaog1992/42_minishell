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

int	ft_builtncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && i < (n - 1))
	{
		if (s1[i] != s2[i] && ft_toupper(s1[i]) != ft_toupper(s2[i]))
			return (1);
		i++;
	}
	return (0);
}

void	ft_call_builtin(char **cmd, char ***envp)
{
	int	i;

	i = 0;
	if (ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
		ft_export(cmd, envp);
	if (ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
		while (cmd[++i])
			if (ft_check_var(cmd[i], cmd[0]))
				ft_unset(cmd[i], envp);
	if (ft_builtncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
		ft_chdir(cmd[1], envp);
	if (ft_builtncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
		ft_env(*envp, 0);
	if (ft_builtncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
		ft_pwd();
	if (ft_builtncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		ft_echo(&cmd[1], *envp);
	if (ft_builtncmp(cmd[0], "$?", ft_strlen(cmd[0])) == 0)
		ft_status(cmd[0]);
}

int	ft_is_builtin(char **cmd)
{
	if (cmd)
	{
		if ((ft_builtncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 3) || (ft_builtncmp(cmd[0], "echo",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 4)
			|| (ft_builtncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 3) || (ft_builtncmp(cmd[0], "$?",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 2))
			return (1);
		if ((ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 6) || (ft_strncmp(cmd[0], "unset",
					ft_strlen(cmd[0])) == 0 && ft_strlen(cmd[0]) == 5)
			|| (ft_builtncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0
				&& ft_strlen(cmd[0]) == 2))
			return (2);
	}
	return (0);
}

char	*ft_subst_var(char *var)
{
	size_t		len;

	len = 0;
	if (var)
	{
		while (var[len] && var[len] != '=')
			len++;
		if (len < ft_strlen(var))
			return (ft_substr(var, 0, len + 1));
	}
	return (0);
}

static void	ft_check_var2(char *var, char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(var, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	status = 1;
}

int	ft_check_var(char *var, char *cmd)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	i = 0;
	if (len == 1 && !ft_isalpha(var[0]))
	{
		ft_check_var2(var, cmd);
		return (0);
	}
	while (var[i] && i < len)
	{
		if (var[i] != '_' && ((!ft_isalpha(var[i]) && i == 0) || \
		!ft_isalnum(var[i])))
		{
			if (!(ft_builtncmp(cmd, "export", ft_strlen(cmd)) == 0 \
			&& var[i] == '=' && !var[i + 1]))
			{
				ft_check_var2(var, cmd);
				return (0);
			}
		}
		i++;
	}
	return (1);
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

int	ft_dup_work(t_fd *fd, int mode)
{
	if (fd->fdin == -1)
	{
		close(fd->fdin);
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		if (mode == 0)
			exit(1);
		return (0);
	}
	if (mode == 1)
		fd->fdin = -2;
	if (fd->fdin >= 0)
	{
		dup2(fd->fdin, STDIN_FILENO);
		close(fd->fdin);
	}
	if (fd->fdout >= 0)
	{
		dup2(fd->fdout, STDOUT_FILENO);
		close(fd->fdout);
	}
	return (1);
}

void	ft_dups(char **redir, t_fd *fd, t_data *node)
{
	int	i;

	i = 0;
	if (redir != NULL)
	{
		while (redir[i])
		{
			if (ft_strncmp(redir[i], "<", ft_strlen(redir[i])) == 0)
				ft_get_fd(redir[i + 1], 0, fd, node);
			else if (ft_strncmp(redir[i], ">", ft_strlen(redir[i])) == 0)
				ft_get_fd(redir[i + 1], 1, fd, node);
			else if (ft_strncmp(redir[i], ">>", ft_strlen(redir[i])) == 0)
				ft_get_fd(redir[i + 1], 2, fd, node);
			else if (ft_strncmp(redir[i], "<<", ft_strlen(redir[i])) == 0)
				ft_get_fd(redir[i + 1], 3, fd, node);
			i++;
		}
	}
}

void	ft_child(t_data *node, char **envp, t_fd *fd, int ret)
{
	ft_dup_work(fd, 0);
	if (node->cmd)
	{
		if (ft_is_builtin(node->cmd))
		{
			ft_call_builtin(node->cmd, &envp);
			ret = 0;
		}
		else
		{
			execve(node->path, node->cmd, envp);
		}
	}
	free(node->path);
	exit(ret);
}

void	ft_pipex(t_data *node, char **envp, t_fd *fd, int ret)
{
	pid_t	pid;

	ft_dups(node->redirection, fd, node);
	pipe(fd->pipe);
	pid = fork();
	if (pid < 0)
		perror("Error");
	if (pid == 0)
	{
		close(fd->pipe[0]);
		dup2(fd->pipe[1], STDOUT_FILENO);
		close(fd->pipe[1]);
		ft_child(node, envp, fd, ret);
	}
	else
	{
		close(fd->pipe[1]);
		dup2(fd->pipe[0], STDIN_FILENO);
		close(fd->pipe[0]);
		ft_close(&fd->fdin, 1);
		ft_close(&fd->fdout, 1);
		waitpid(pid, &ret, 0);
	}
}

int	ft_check_cmd(t_data *node, t_fd *fd, int *ret, int mode)
{
	if (node->path || ft_is_builtin(node->cmd))
		return (1);
	if (node->cmd)
	{
		if (ft_strncmp(node->cmd[0], "exit", ft_strlen(node->cmd[0])) == 0
			&& ft_strlen(node->cmd[0]) == 4)
			return (0);
		*ret = 32512;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->cmd[0], 2);
		ft_putendl_fd(" command not found", 2);
		ft_close(&fd->fdin, 1);
		ft_close(&fd->fdout, 1);
	}
	ft_dups(node->redirection, fd, node);
	if (mode == 1)
		close(STDIN_FILENO);
	ft_reset_fd(fd);
	return (0);
}

void	ft_get_fd(char *file, int mode, t_fd *fd, t_data *node)
{
	if (mode == 0)
	{
		ft_close(&fd->fdin, 0);
		fd->fdin = open(file, O_RDONLY);
	}
	else if (mode == 1)
	{
		ft_close(&fd->fdout, 0);
		fd->fdout = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
	else if (mode == 2)
	{
		ft_close(&fd->fdout, 0);
		fd->fdout = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	else if (mode == 3)
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

void	ft_close_all(t_fd *fd)
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

static int	ft_single_builtin(t_data *node, t_fd fd, char ***envp, int node_nb)
{
	if (node_nb == 1 && ft_is_builtin(node->cmd) == 2)
	{
		ft_dups(node->redirection, &fd, node);
		ft_close(&fd.fdin, 0);
		if (ft_dup_work(&fd, 1))
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
			ft_dups(node->redirection, fd, node);
			ft_child(node, *envp, fd, ret);
		}
		else
			exit(127);
	}
	else
		waitpid(pid, &ret, 0);
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
				if (ft_strncmp(node->redirection[j], "<<", \
				ft_strlen(node->redirection[j])) == 0)
					here_doc(node->redirection[j + 1], node);
				j++;
			}
		}
		node = node->next;
		i++;
	}
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
	status = ft_exec_loop(node_nb, &fd, envp, node);
	ft_close_all(&fd);
	ft_reset_fd(&fd);
}