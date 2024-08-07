/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_green.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 12:57:25 by marvin            #+#    #+#             */
/*   Updated: 2023/08/18 12:57:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_print_green(char *s)
{
	return (ft_printf("\033[0;92m%s\033[0;97m\n", s));
}
