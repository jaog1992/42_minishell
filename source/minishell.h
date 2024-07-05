/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 09:11:00 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/22 09:11:00 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/source/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define TRUE 1
# define FALSE 0
# define RED "\001\e[0;91m\002"
# define GREEN "\001\e[0;92m\002"
# define BLUE  "\001\e[0;94m\002"
# define DEF_COLOR "\001\e[0;39m\002"

typedef struct s_data
{
	char			**cmd;
	char			**redirection;
	char			*path;
	int				input;
	int				output;
	char			**tokens;
	int				here_doc;
	struct s_data	*next;
}			t_data;

typedef struct s_minishell
{
	char	**env;
    char	**tokens;
	int		status;
}			t_minishell;

typedef struct s_fd
{
	int		in;
	int		out;
	int		fdin;
	int		fdout;
	int		pipe[2];
	int		here_doc;
	char	*key;
}			t_fd;

// Utils
// Utils - str

// Utils - malloc

// Signals
void	rl_replace_line(const char *text, int clear_undo);
void	signal_handler(int num);
void	ft_signals(void);
void	signal_handler_in_cat(int num);
void	ft_signals_in_cat(void);

// Exit function
int		ft_exit(char *str);

char	*ft_getenvval(char** env, char *str);
// Expansor functions. They expand env variable values
int		dollar_var_len(char *str);
int		ft_next_same_char_pos(char *str, char a);
char	*dollar_variable(char *str);
int		find_pos(char *str, int x);
char	*check_var(char *var, char *one, char *second, t_minishell *mshell);
int		ft_variable_expansion(char **str, int x, t_minishell *mshell);
int		ft_closing_char(char *str, char c);
int		check_next_char(char c);
char	*ft_variable_expansion_check(char *str, t_minishell *minishell);

// Tokens

int		check_assign(int check, char a, char **quote, char *c);
void	count_assign(char **aux, char **quote, int *x, char a);
int		ft_isdupchar(char *str);
int		ft_word_length(char *str);
int		ft_count_words(char *str);
void	fill_array1(char *str, char **tokens);
void	fill_map(char *quote, char *no, char **fill);
void	fill_array2(char **tokens);
char	**ft_get_tokens(char *str, t_minishell *minishell);
int		ft_count_tokens(char *str);

// General

void	ft_lstclear1(t_data **lst);
int		check_redirection1(char **ptr);
int		ft_general_function(char *str, t_minishell *minishell);
// Command execution

int		ft_builtncmp(const char *s1, const char *s2, size_t n);
void	ft_call_builtin(char **cmd, char ***envp);
int		ft_is_builtin(char **cmd);
char	*ft_subst_var(char *var);
//static void	ft_check_var2(char *var, char *cmd);
int		ft_check_var(char *var, char *cmd);
int		ft_count_nodes(t_data *node);
int		ft_dup_work(t_fd *fd, int mode);
void	ft_dups(char **redir, t_fd *fd, t_data *node);
void	ft_child(t_data *node, char **envp, t_fd *fd, int ret);
void	ft_pipex(t_data *node, char **envp, t_fd *fd, int ret);
int		ft_check_cmd(t_data *node, t_fd *fd, int *ret, int mode);
void	ft_get_fd(char *file, int mode, t_fd *fd, t_data *node);
void	ft_close(int *fd, int mode);
void	ft_init_fd(t_fd *fd);
void	ft_close_all(t_fd *fd);
void	ft_reset_fd(t_fd *fd);
void	ft_init_fd(t_fd *fd);
//static int	ft_single_builtin(t_data *node, t_fd fd, char ***envp, int node_nb);
//static int	ft_exec_loop(int node_nb, t_fd *fd, char ***envp, t_data *node);
void	ft_fill_here_doc(t_data *node, int node_nb);
void	ft_exec(t_data *node, char ***envp);

#endif