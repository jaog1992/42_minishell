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

# include "../libft/source/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

//BOOLEAN DEFINES
# define TRUE 1
# define FALSE 0

// OUTPUT COLOR DEFINES
# define RED "\001\e[0;91m\002"
# define GREEN "\001\e[0;92m\002"
# define BLUE  "\001\e[0;94m\002"
# define DEF_COLOR "\001\e[0;39m\002"

// COMMAND EXIT STATUS DEFINES (g_status)
# define CMD_NOT_FOUND 127
# define CMD_SUCCESS 0
# define CMD_GENERIC_ERROR 1

// REDIRECTION TYPE DEFINES
# define INPUT_REDIRECTION 0
# define OUTPUT_REDIRECTION 1
# define OUTPUT_REDIRECTION_APPEND 2
# define HERE_DOCUMENT 3

// FT_ENV MODES
# define ENV_BASIC_FORMAT 0
# define ENV_EXPORT_FORMAT 1

extern int	g_status;

typedef struct s_data
{
	int				input;
	int				output;
	int				here_doc;
	char			*path;
	char			**cmd;
	char			**tokens;
	char			**redirection;
	struct s_data	*next;
}			t_data;

typedef struct s_minishell
{
	char	**env;
	char	**tokens;
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


// Signals
void	rl_replace_line(const char *text, int clear_undo);
void	signal_handler(int num);
void	ft_signals(void);
void	signal_handler_in_cat(int num);
void	ft_signals_in_cat(void);

// status
int		ft_status(char *str);
// Exit function
int		ft_exit(char *str);
char	*ft_getenvval(char **env, char *str);

// Expansor functions. They expand env variable values
int		dollar_var_len(char *str);
char	*dollar_variable(char *str);
int		find_pos(char *str, int x);
char	*check_var(char *var, char *one, char *second, t_minishell *mshell);
int		ft_variable_expansion(char **str, int x, t_minishell *mshell);
int		ft_closing_char(char *str, char c, int ret);
int		check_next_char(char c);
char	*ft_variable_expansion_check(char *str, t_minishell *minishell);

// Tokens

int		check_assign(int check, char a, char **quote, char *c);
void	count_assign(char **aux, char **quote, int *x, char a);
int		ft_isdupchar(char *str);
char	*erase_quotes(char *str);
void	token_without_quotes(char *quoted_str, char *unquoted_str, char **fill);
int		ft_count_chars(char *str);
int		ft_count_words(char *str);
void	fill_tokens(char *str, char **tokens);
void	fill_map(char *quote, char *no, char **fill);
void	erase_tokens_quotes(char **tokens);
char	**ft_get_tokens(char *str);
int		ft_count_tokens(char *str);

// General

void	ft_freelistdata(t_data **lst);
int		check_invalid_redirection(char **ptr);
int		ft_lexer(char *str, t_minishell *minishell, t_data **data);
// Command execution

int		ft_builtncmp(const char *s1, const char *s2, size_t n);
void	ft_call_builtin(char **cmd, char ***envp);
char	*ft_subst_var(char *var);
void	fill_cmd_path(t_data *list, char **envp);
int		second_char_exists(char *str, char a);
char	*check_str(char *f, char *str, char *cmd, char ***p);
char	*check_if_command(char **envp, char *str);

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
void	ft_close_fd(t_fd *fd);
void	ft_reset_fd(t_fd *fd);
void	ft_init_fd(t_fd *fd);
void	ft_fill_here_doc(t_data *node, int node_nb);
void	ft_exec(t_data *node, char ***envp);

//redirection
t_data	*ft_redirection(char **tokens);
t_data	*init_data(void);
t_data	*ft_listlastnode(t_data *lst);
void	ft_listaddnodetoend(t_data **lst, t_data *new);
int		check_invalid_pipe(char **tokens);

//commands
t_data	*ft_commands(char **tokens, t_data *aux);
void	here_doc(char *key, t_data *node);

// builtins
void	ft_pwd(void);
void	ft_chdir(char *dir, char ***env);
void	ft_export(char **cmd, char ***env);
void	ft_update_env_var(char *var, char *value, char ***env);
void	ft_env(char **env, int mode);
void	ft_unset(char *var, char ***env);
void	ft_echo(char **cmd, char **env);
void	ft_history(void);

//builtins commands

int		ft_builtncmp(const char *s1, const char *s2, size_t n);
void	ft_call_builtin(char **cmd, char ***envp);
int		ft_is_builtin(char **cmd);
#endif