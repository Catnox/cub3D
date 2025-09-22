/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	handle_mouse(double xpos, double ypos, void *param)
{
	t_game		*game;
	static double	last_x = WINDOW_WIDTH / 2;
	double		delta_x;
	
	game = (t_game *)param;
	(void)ypos;
	delta_x = xpos - last_x;
	last_x = xpos;
	if (delta_x != 0)
		rotate_player(game, delta_x * 0.001);
}

void	handle_mouse_click(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_game	*game;
	
	game = (t_game *)param;
	(void)mods;
	if (action == MLX_PRESS)
	{
		if (button == MLX_MOUSE_BUTTON_LEFT)
			enhanced_shoot_weapon(game);
		else if (button == MLX_MOUSE_BUTTON_RIGHT)
			reload_weapon(game);
	}
}
