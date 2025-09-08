/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static char	**read_file_lines(int fd)
{
	char	**lines;
	char	*line;
	int		capacity;
	int		count;
	
	capacity = 100;
	count = 0;
	lines = malloc(sizeof(char *) * capacity);
	if (!lines)
		return (NULL);
	
	while ((line = get_next_line(fd)) != NULL)
	{
		if (count >= capacity - 1)
		{
			capacity *= 2;
			lines = realloc(lines, sizeof(char *) * capacity);
			if (!lines)
				return (NULL);
		}
		
		// Remove newline if present
		int len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		
		lines[count++] = line;
	}
	lines[count] = NULL;
	return (lines);
}

static void	free_lines(char **lines)
{
	int	i;
	
	if (!lines)
		return;
	
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

int	parse_cub_file(t_game *game, char *filename)
{
	int		fd;
	char	**lines;
	size_t	len;
	int		i;
	
	// Validate .cub extension
	len = ft_strlen(filename);
	if (len < 4 || ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (error_exit("Invalid file extension. Expected .cub"), 1);
	
	// Open file
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (error_exit("Failed to open map file"), 1);
	
	// Read all lines
	lines = read_file_lines(fd);
	close(fd);
	if (!lines)
		return (error_exit("Failed to read file"), 1);
	
	// Allocate structures
	game->map = ft_calloc(1, sizeof(t_map));
	game->player = ft_calloc(1, sizeof(t_player));
	game->textures = ft_calloc(1, sizeof(t_texture));
	
	if (!game->map || !game->player || !game->textures)
		return (free_lines(lines), error_exit("Memory allocation failed"), 1);
	
	// Parse textures and colors first
	i = 0;
	while (lines[i])
	{
		if (parse_textures(game, lines[i]) != 0 || parse_colors(game, lines[i]) != 0)
		{
			free_lines(lines);
			return (1);
		}
		i++;
	}
	
	// Parse the map
	if (parse_map(game, lines) != 0)
	{
		free_lines(lines);
		return (1);
	}
	
	free_lines(lines);
	return (validate_map(game->map));
}
