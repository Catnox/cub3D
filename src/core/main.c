/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/17 14:29:36 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Main entry point for the cub3D application
 * 
 * This function initializes the game state, loads the specified map file,
 * and starts the main game loop. It handles command-line arguments to ensure
 * a map file is provided, and performs cleanup on exit.
 * 
 * @param argc Argument count
 * @param argv Argument vector (expects map file as argv[1])
 * @return Exit status code (0 on success, 1 on error)
 */
int	main(int argc, char **argv)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
		return (1);
	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (1);
	}
	if (init_game(game, argv[1]) != 0)
	{
		cleanup_game(game);
		return (1);
	}
	game_loop(game);
	// mlx_loop() returns after window is closed
    printf("DEBUG: mlx_loop ended, now terminating MLX\n");
    mlx_terminate(game->mlx);
    free(game);
    printf("DEBUG: full cleanup done\n");
	return (0);
}
