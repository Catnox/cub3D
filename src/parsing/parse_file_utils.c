/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/27 14:36:50 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Read all lines from a file descriptor into a dynamic array
 * 
 * Reads the entire file line by line using get_next_line and stores
 * each line in a dynamically growing array. The array automatically
 * resizes when capacity is exceeded.
 * 
 * @param fd File descriptor to read from
 * @return NULL-terminated array of strings, or NULL on failure
 */
char	**read_file_lines(int fd)
{
	char	**lines;
	char	*line;
	int		capacity;
	int		count;

	capacity = 16;
	count = 0;
	lines = malloc(sizeof(char *) * capacity);
	if (!lines)
		return (NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (count >= capacity - 1)
		{
			lines = resize_lines_array(lines, &capacity, count);
			if (!lines)
				return (NULL);
		}
		process_line(line);
		lines[count++] = line;
		line = get_next_line(fd);
	}
	lines[count] = NULL;
	return (lines);
}

int	validate_cub_extension(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (1);
	return (0);
}

char	**open_and_read_file(char *filename)
{
	int		fd;
	char	**lines;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		error_exit("Failed to open map file");
		return (NULL);
	}
	lines = read_file_lines(fd);
	close(fd);
	if (!lines)
	{
		error_exit("Failed to read file");
		return (NULL);
	}
	return (lines);
}
