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
	int		g_status;
}			t_minishell;

// Utils
// Utils - str

// Utils - malloc

// Signals
void	rl_replace_line(const char *text, int clear_undo);
void	signal_handler(int num);
void	ft_signals(void);
void	signal_handler_in_cat(int num);
void	ft_signals_in_cat(void);

// exit function
int		ft_exit(char *str);

char	*ft_getenvval(char** env, char *str);
// expansor functions. They expand env variable values
int		dollar_var_len(char *str);
int		count_char_index(char *str, char a);
char	*dollar_variable(char *str);
int		find_pos(char *str, int x);
char	*check_var(char *var, char *one, char *second, t_minishell *mshell);
int		ft_variable_expansion(char **str, int x, t_minishell *mshell);
int		closing_char_exists(char *str, char c);
int		check_next_char(char c);
char	*ft_variable_expansion_check(char *str, t_minishell *minishell);
#endif