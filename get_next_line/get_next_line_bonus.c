/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:56:12 by radubos           #+#    #+#             */
/*   Updated: 2024/12/27 13:00:15 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*tank;
	char		*buffer;
	ssize_t		read_bytes;

	if (fd < 0 || BUFFER_SIZE < 1)
	{
		ft_free(&tank);
		return (NULL);
	}
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes == -1)
	{
		free(buffer);
		ft_free(&tank);
		return (NULL);
	}
	buffer[read_bytes] = '\0';
	if (read_bytes == 0)
		return (end_of_read(&tank, &buffer));
	return (fill_tank(&tank, &buffer, fd));
}

char	*fill_tank(char **tank, char **buffer, int fd)
{
	char	*line;
	char	*temp;

	temp = *tank;
	*tank = ft_strjoin(temp, *buffer);
	free(temp);
	free(*buffer);
	if (ft_strchr(*tank, '\n'))
	{
		temp = *tank;
		line = extract_line(temp, '\n');
		*tank = update_tank(temp, '\n');
		free(temp);
		return (line);
	}
	return (get_next_line(fd));
}

char	*end_of_read(char **tank, char **buffer)
{
	char	*line;
	char	*temp;

	free(*buffer);
	if (ft_strlen(*tank))
	{
		if (ft_strchr(*tank, '\n'))
		{
			temp = *tank;
			line = extract_line(temp, '\n');
			*tank = update_tank(temp, '\n');
			free(temp);
			return (line);
		}
		line = extract_line(*tank, '\n');
		free(*tank);
		*tank = NULL;
		return (line);
	}
	free(*tank);
	*tank = NULL;
	return (*tank);
}

void	ft_free(char **tank)
{
	if (*tank)
	{
		free(*tank);
		*tank = NULL;
	}
}
