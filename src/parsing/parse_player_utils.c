/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/20 10:48:01 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_north_direction(t_player *player)
{
	player->dir.x = 0.0;
	player->dir.y = -1.0;
	player->plane.x = 0.66;
	player->plane.y = 0.0;
}

void	set_south_direction(t_player *player)
{
	player->dir.x = 0.0;
	player->dir.y = 1.0;
	player->plane.x = -0.66;
	player->plane.y = 0.0;
}

void	set_east_direction(t_player *player)
{
	player->dir.x = 1.0;
	player->dir.y = 0.0;
	player->plane.x = 0.0;
	player->plane.y = 0.66;
}

void	set_west_direction(t_player *player)
{
	player->dir.x = -1.0;
	player->dir.y = 0.0;
	player->plane.x = 0.0;
	player->plane.y = -0.66;
}

void	set_player_direction(t_player *player, char dir)
{
	if (dir == 'N')
		set_north_direction(player);
	else if (dir == 'S')
		set_south_direction(player);
	else if (dir == 'E')
		set_east_direction(player);
	else if (dir == 'W')
		set_west_direction(player);
}
