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

/**
 * Resize the lines array when capacity is exceeded
 * 
 * Doubles the capacity of the lines array using malloc and manual copying
 * to accommodate more lines when the current capacity is reached.
 * This approach avoids realloc for better control and 42 compatibility.
 * 
 * @param lines Pointer to the current lines array
 * @param capacity Pointer to current capacity (will be doubled)
 * @param count Current number of lines in the array
 * @return Pointer to resized array, or NULL on failure
 */
static char	**resize_lines_array(char **lines, int *capacity, int count)
{
	char	**new_lines;
	int		i;
	int		new_capacity;

	new_capacity = (*capacity) * 2;
	new_lines = malloc(sizeof(char *) * new_capacity);
	if (!new_lines)
	{
		free(lines);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_lines[i] = lines[i];
		i++;
	}
	free(lines);
	*capacity = new_capacity;
	return (new_lines);
}

static void	process_line(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

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
static char	**read_file_lines(int fd)
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
	//fix
	while ((line = get_next_line(fd)) != NULL)
	{
		if (count >= capacity - 1)
		{
			lines = resize_lines_array(lines, &capacity, count);
			if (!lines)
				return (NULL);
		}
		process_line(line);
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

static int	validate_cub_extension(char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (error_exit("Invalid file extension. Expected .cub"), 1);
	return (0);
}

static char	**open_and_read_file(char *filename)
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

static int	allocate_game_structures(t_game *game)
{
	game->map = ft_calloc(1, sizeof(t_map));
	game->player = ft_calloc(1, sizeof(t_player));
	game->textures = ft_calloc(1, sizeof(t_texture));
	if (!game->map || !game->player || !game->textures)
	{
		if (game->map)
			free(game->map);
		if (game->player)
			free(game->player);
		if (game->textures)
			free(game->textures);
		return (1);
	}
	return (0);
}

/**
 * Parse and load a .cub map file for the cub3D game
 * 
 * This function is the main entry point for parsing .cub files and performs
 * a complete validation and loading process. The .cub file format contains:
 * - Texture paths for the four wall directions (NO, SO, EA, WE)
 * - Floor and ceiling colors (F, C) in RGB format
 * - A 2D map layout using characters (0=floor, 1=wall, N/S/E/W=player start, etc.)
 * 
 * @param game Pointer to the main game structure to populate
 * @param filename Path to the .cub file to parse
 * @return 0 on successful parsing and validation, 1 on any error
 */
int	parse_cub_file(t_game *game, char *filename)
{
	char	**lines;
	int		i;
	
	if (validate_cub_extension(filename) != 0)
		return (1);
	lines = open_and_read_file(filename);
	if (!lines)
		return (1);
	if (allocate_game_structures(game) != 0)
		return (free_lines(lines), error_exit("Memory allocation failed"), 1);
	i = 0;
	while (lines[i])
	{
		if (parse_textures(game, lines[i]) != 0 || parse_colors(game, lines[i]) != 0)
			return (free_lines(lines), cleanup_game(game), 1);
		i++;
	}
	if (parse_map(game, lines) != 0)
		return (free_lines(lines), cleanup_game(game), 1);
	free_lines(lines);
	return (validate_map(game->map));
}
