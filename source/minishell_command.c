/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:30:28 by jde-orma          #+#    #+#             */
/*   Updated: 2024/08/02 16:30:28 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/source/libft.h"

int	second_char_exists(char *str, char a)
{
	str++;
	while (*str)
	{
		if (*str == a)
			return (1);
		str++;
	}
	return (0);
}

//Comentado de forma provisional
// char	*find_path(char **envp)
// {
// 	while (*envp)
// 	{
// 		if (ft_strnstr(*envp, "PATH=", 5))
// 			return (ft_strchr(*envp, '/'));
// 		envp++;
// 	}
// 	return (0);
// }

char	*check_str(char *f, char *str, char *cmd, char ***p)
{
	if (access(str, F_OK) == 0)
	{
		free(cmd);
		free(f);
		ft_free2dstr(*p);
		return (ft_strdup(str));
	}
	else if (access(f, F_OK) == 0)
	{
		free(cmd);
		ft_free2dstr(*p);
		return (f);
	}
	free(f);
	return (NULL);
}

char	*check_if_command(char **envp, char *str)
{
	int		i;
	char	**path_list;
	char	*cmd;
	char	*full_cmd_path;

	i = 0;
	if (!str)
		return (0);
	if (!access(str, F_OK))
		return (ft_strdup(str));
	cmd = ft_strjoin("/", str);
	path_list = ft_split(ft_getenvval(envp, "PATH"), ':');
	while (path_list && path_list[i])
	{
		full_cmd_path = ft_strjoin(path_list[i], cmd);
		full_cmd_path = check_str(full_cmd_path, str, cmd, &path_list);
		if (full_cmd_path != NULL)
			return (full_cmd_path);
		free(full_cmd_path);
		i++;
	}
	free(cmd);
	if (path_list)
		ft_free2dstr(path_list);
	return (0);
}

void	fill_cmd_path(t_data *list, char **envp)
{
	char	*path;

	path = NULL;
	while (TRUE)
	{
		if (list->cmd)
		{
			if (list->cmd && !ft_is_builtin(list->cmd))
				path = check_if_command(envp, list->cmd[0]);
			if (path != 0)
				list->path = path;
		}
		if (ft_is_builtin(list->cmd) > 0)
			list->path = NULL;
		if (list->next == NULL)
			break ;
		list = list->next;
	}
}