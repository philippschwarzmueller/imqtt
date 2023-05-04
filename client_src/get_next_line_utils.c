/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 16:13:47 by makurz            #+#    #+#             */
/*   Updated: 2023/04/11 16:52:33 by makurz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_clean_stash(char *str, t_gnl next)
{
	next.line_bool = false;
	next.i = 0;
	next.k = 0;
	while (str[next.i])
	{
		if (next.line_bool == true)
			str[next.k++] = str[next.i];
		if (str[next.i] == '\n')
			next.line_bool = true;
		str[next.i++] = '\0';
	}
	return (next.line_bool);
}

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2, t_gnl str)
{
	str.new_line = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (str.new_line == NULL)
		return (NULL);
	str.i = 0;
	str.k = 0;
	while (s1 && s1[str.i])
	{
		str.new_line[str.i] = s1[str.i];
		str.i++;
	}
	while (s2 && s2[str.k])
	{
		str.new_line[str.i] = s2[str.k++];
		if (str.new_line[str.i++] == '\n')
			break ;
	}
	str.new_line[str.i] = '\0';
	free (s1);
	return (str.new_line);
}
