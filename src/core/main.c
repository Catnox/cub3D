/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:18:49 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (1);
	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (free(game), 1);
	}
	if (init_game(game, argv[1]) != 0)
	{
		cleanup_game(game);
		free(game);
		printf("DEBUG: cleanup done after init failure\n");
		return (1);
	}
	game_loop(game);
	return (0);
}
