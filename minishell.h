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

#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

// Utils
// Utils - str
int     ft_strlen(const char *str);
int     ft_dblstrlen(char **str);
char    *ft_strdup(char *str);
char    **ft_dblstrdup(char **str);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char const *s1, char const *s2);
int	    ft_char_exists(char c, char const *set);
char	*ft_strtrim(char const *s1, char const *set);

// Utils - malloc
void    ft_free(char **str);

// Signals
void	rl_replace_line(const char *text, int clear_undo);
void	signal_handler(int num);
void	ft_signals(void);
void	signal_handler_in_cat(int num);
void	ft_signals_in_cat(void);

#endif