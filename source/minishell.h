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

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define TRUE 1
# define FALSE 0
// Utils
// Utils - str

// Utils - malloc

// Signals
void	rl_replace_line(const char *text, int clear_undo);
void	signal_handler(int num);
void	ft_signals(void);
void	signal_handler_in_cat(int num);
void	ft_signals_in_cat(void);

#endif