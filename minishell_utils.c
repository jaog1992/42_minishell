/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-orma <jde-orma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 09:09:14 by jde-orma          #+#    #+#             */
/*   Updated: 2024/06/22 09:09:14 by jde-orma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize != 0)
	{
		while (src [i] && i < (dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}


size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_length;
	size_t	dst_length;

	i = 0;
	src_length = (size_t)ft_strlen(src);
	dst_length = (size_t)ft_strlen(dst);
	if (dstsize == 0 || dst_length >= dstsize)
		return (src_length + dstsize);
	while (src[i] && (dstsize > dst_length + i + 1))
	{
		dst[dst_length + i] = src[i];
		i++;
	}
	dst[dst_length + i] = '\0';
	return (dst_length + src_length);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * (len));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, len);
	ft_strlcat(ptr, s2, len);
	return (ptr);
}

void ft_free(char **str)
{
    int i;

    i = 0;
    if(!str)
        return ;
    while(str[i] != NULL)
        free(str[i++]);
    free(str);
    return ;
}

int ft_strlen(const char *str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    while(str[i])
        i++;
    return (i);
}

int ft_dblstrlen(char **str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    while(str[i] != NULL)
        i++;
    return (i);
}

char *ft_strdup(char *str)
{
    int i;
    char *str2;

    if (!str)
        return (NULL);
    i = ft_strlen(str);
    str2 = (char *)malloc(sizeof(char) * (i + 1));
    if (!str2)
        return (NULL);
    i = 0;
    while (str[i])
        str2[i++] = str[i];
    str2[i] = '\0';
    return (str2);
}

char **ft_dblstrdup(char **str)
{
    int i;
    int len;
    char **dblstrdup;

    len = ft_dblstrlen(str);
    dblstrdup = (char**)malloc(sizeof(char*) * (len + 1));
    dblstrdup[len--] = NULL;
    while(len >= 0)
    {
        i = ft_strlen(str[len]);
        dblstrdup[len--] = ft_strdup(str[len]);
    }
    return (dblstrdup);
}
