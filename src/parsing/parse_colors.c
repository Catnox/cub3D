/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	parse_rgb_values(char *rgb_str, t_color *color)
{
	char	**rgb_split;
	int		r;
	int		g;
	int		b;
	
	rgb_split = ft_split(rgb_str, ',');
	if (!rgb_split || !rgb_split[0] || !rgb_split[1] || !rgb_split[2] || rgb_split[3])
	{
		free_split(rgb_split);
		return (error_exit("Invalid RGB format"), 1);
	}
	
	r = ft_atoi(rgb_split[0]);
	g = ft_atoi(rgb_split[1]);
	b = ft_atoi(rgb_split[2]);
	
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		free_split(rgb_split);
		return (error_exit("RGB values must be between 0 and 255"), 1);
	}
	
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = 255; // Full opacity
	
	free_split(rgb_split);
	return (0);
}

int	parse_colors(t_game *game, char *line)
{
	char	*trimmed;
	
	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (1);
	
	if (ft_strncmp(trimmed, "F ", 2) == 0)
	{
		if (parse_rgb_values(trimmed + 2, &game->map->floor_color) != 0)
		{
			free(trimmed);
			return (1);
		}
	}
	else if (ft_strncmp(trimmed, "C ", 2) == 0)
	{
		if (parse_rgb_values(trimmed + 2, &game->map->ceiling_color) != 0)
		{
			free(trimmed);
			return (1);
		}
	}
	
	free(trimmed);
	return (0);
}
