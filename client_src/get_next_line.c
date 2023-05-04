/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makurz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 16:13:31 by makurz            #+#    #+#             */
/*   Updated: 2023/04/11 14:02:49 by makurz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	stash[BUFFER_SIZE + 1];
	t_gnl		next;

	next.new_line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	while (stash[0] != '\0' || read(fd, stash, BUFFER_SIZE) > 0)
	{
		next.new_line = ft_strjoin(next.new_line, stash, next);
		if (next.new_line == NULL)
			return (NULL);
		if (ft_clean_stash(stash, next) == true)
			break ;
		if (read(fd, stash, 0) < 0)
			return (free(next.new_line), NULL);
	}
	return (next.new_line);
}
