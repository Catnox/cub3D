/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 11:04:03 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->dir.x;
	game->player->dir.x = game->player->dir.x * cos(angle)
		- game->player->dir.y * sin(angle);
	game->player->dir.y = old_dir_x * sin(angle)
		+ game->player->dir.y * cos(angle);
	old_plane_x = game->player->plane.x;
	game->player->plane.x = game->player->plane.x * cos(angle)
		- game->player->plane.y * sin(angle);
	game->player->plane.y = old_plane_x * sin(angle)
		+ game->player->plane.y * cos(angle);
}
