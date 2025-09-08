#include "../../includes/cub3d.h"

// Rendu de l'écran de game over
void	render_game_over_screen(t_game *game)
{
	int x, y;
	uint32_t red_color = 0xFF000080; // Rouge semi-transparent
	uint32_t text_color = 0xFFFFFFFF; // Blanc
	int center_x = game->mlx->width / 2;
	int center_y = game->mlx->height / 2;
	
	if (!game->game_over)
		return;
	
	// Overlay rouge semi-transparent
	y = 0;
	while (y < (int)game->mlx->height)
	{
		x = 0;
		while (x < (int)game->mlx->width)
		{
			mlx_put_pixel(game->img, x, y, red_color);
			x++;
		}
		y++;
	}
	
	// Dessine "GAME OVER" simplement
	draw_simple_game_over(game, center_x - 50, center_y - 40);
	
	// Dessine le score
	char score_text[50];
	snprintf(score_text, sizeof(score_text), "Score: %d", game->player->score);
	draw_simple_message(game, center_x - 40, center_y, score_text);
	
	// Instructions
	draw_simple_message(game, center_x - 80, center_y + 40, "Press R to restart");
	draw_simple_message(game, center_x - 70, center_y + 60, "Press ESC to exit");
}

// Logique principale du game over
void	handle_game_over(t_game *game)
{
	if (game->player->hp <= 0 && !game->game_over)
	{
		game->game_over = 1;
		printf("GAME OVER! Final Score: %d\n", game->player->score);
		printf("Press R to restart or ESC to exit\n");
		
		// Arrête tous les timers et animations
		game->weapon->is_shooting = 0;
		game->weapon->animation_timer = 0.0;
		game->weapon->current_frame = 0;
		
		// Arrête le mouvement des ennemis
		int i = 0;
		while (i < game->enemies->count)
		{
			if (game->enemies->enemies[i].is_alive)
				game->enemies->enemies[i].state = 3; // Force dead state
			i++;
		}
	}
}

// Gère les inputs pendant le game over
void	handle_game_over_input(t_game *game, mlx_key_data_t keydata)
{
	if (!game->game_over)
		return;
		
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_R)
		{
			// Restart game
			restart_game(game);
		}
		else if (keydata.key == MLX_KEY_ESCAPE)
		{
			// Exit game
			printf("Exiting game...\n");
			mlx_close_window(game->mlx);
		}
	}
}

// Redémarre le jeu
void	restart_game(t_game *game)
{
	printf("Restarting game...\n");
	
	// Reset player
	game->player->hp = 100;
	game->player->ammo = 30;
	game->player->score = 0;
	
	// Reset player position (trouve la position de spawn sur la map)
	reset_player_position(game);
	
	// Reset weapon
	game->weapon->current_frame = 0;
	game->weapon->is_shooting = 0;
	game->weapon->animation_timer = 0.0;
	
	// Reset enemies (respawn)
	cleanup_enemies(game);
	init_enemies(game);
	
	// Reset game state
	game->game_over = 0;
	
	printf("Game restarted!\n");
}

// Trouve et reset la position de spawn du joueur
void	reset_player_position(t_game *game)
{
	int x, y;
	
	// Cherche la position de spawn sur la map
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			char cell = game->map->grid[y][x];
			if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
			{
				// Position trouvée
				game->player->pos.x = (double)x + 0.5;
				game->player->pos.y = (double)y + 0.5;
				
				// Direction selon l'orientation
				switch (cell)
				{
					case 'N':
						game->player->dir.x = 0.0;
						game->player->dir.y = -1.0;
						game->player->plane.x = 0.66;
						game->player->plane.y = 0.0;
						break;
					case 'S':
						game->player->dir.x = 0.0;
						game->player->dir.y = 1.0;
						game->player->plane.x = -0.66;
						game->player->plane.y = 0.0;
						break;
					case 'E':
						game->player->dir.x = 1.0;
						game->player->dir.y = 0.0;
						game->player->plane.x = 0.0;
						game->player->plane.y = 0.66;
						break;
					case 'W':
						game->player->dir.x = -1.0;
						game->player->dir.y = 0.0;
						game->player->plane.x = 0.0;
						game->player->plane.y = -0.66;
						break;
				}
				return;
			}
			x++;
		}
		y++;
	}
}

// Stubs pour les fonctions de texte simplifiées
void	draw_simple_game_over(t_game *game, int start_x, int start_y)
{
	(void)game;
	(void)start_x;
	(void)start_y;
	// Stub - pas de rendu de texte complexe
}

void	draw_simple_message(t_game *game, int start_x, int start_y, const char *message)
{
	(void)game;
	(void)start_x;
	(void)start_y;
	(void)message;
	// Stub - pas de rendu de texte complexe
}
