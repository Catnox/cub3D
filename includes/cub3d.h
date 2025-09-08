/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../libs/MLX42/include/MLX42/MLX42.h"
# include <math.h>
# include <fcntl.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 768
# define TEXTURE_SIZE 64
# define MOVE_SPEED 0.08
# define ROT_SPEED 0.025
# define FOV 66
# define MAX_ENEMIES 50
# define MAX_ITEMS 100
# define ENEMY_SIZE 0.8
# define ENEMY_SPEED 0.02
# define ENEMY_DAMAGE 25
# define ENEMY_HEALTH 50

/* Forward declarations */
typedef struct s_node	t_node;

/* Colors */
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	a;
}	t_color;

/* Vector 2D */
typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

/* Textures */
typedef struct s_texture
{
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
	mlx_texture_t	*door;
}	t_texture;

/* Door */
typedef struct s_door
{
	int		x;
	int		y;
	double	open_state;  // 0.0 = closed, 1.0 = fully open
	int		is_opening;  // 1 = opening, -1 = closing, 0 = static
	double	open_speed;  // Speed of opening/closing animation
}	t_door;

/* Map */
typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	t_color		floor_color;
	t_color		ceiling_color;
	t_texture	textures;
	t_door		*doors;
	int			door_count;
}	t_map;

/* HUD */
typedef struct s_hud
{
	mlx_image_t	*crosshair;
	mlx_image_t	*health_bar;
	mlx_image_t	*ammo_counter;
	mlx_image_t	*score_display;
	int			crosshair_size;
	int			crosshair_thickness;
	t_color		crosshair_color;
}	t_hud;

/* Weapon */
typedef struct s_weapon
{
	mlx_image_t	*frames[10];  // Animation frames
	int			current_frame;
	int			frame_count;
	int			is_shooting;
	double		animation_timer;
	double		frame_duration;
	int			damage;
	int			ammo_per_shot;
}	t_weapon;

/* Enemy */
typedef struct s_enemy
{
	t_vec2		pos;
	t_vec2		dir;
	int			hp;
	int			max_hp;
	int			is_alive;
	int			is_attacking;
	double		attack_timer;
	double		move_timer;
	int			enemy_type;  // 0 = guard, 1 = officer, 2 = dog, etc.
	double		speed;
	int			damage;
	double		detection_range;
	int			state;  // 0 = patrol, 1 = alert, 2 = attacking, 3 = dead
	t_vec2		target_pos;  // For pathfinding
	t_node		**path;      // Chemin A* vers le joueur
	int			path_length; // Longueur du chemin
	int			path_step;   // Étape actuelle dans le chemin
	double		pathfind_timer; // Timer pour recalculer le chemin
}	t_enemy;

/* Enemies system */
typedef struct s_enemies
{
	t_enemy		enemies[MAX_ENEMIES];
	int			count;
	double		spawn_timer;
	mlx_texture_t	*texture_guard;
	mlx_texture_t	*texture_officer;
	mlx_texture_t	*texture_dog;
}	t_enemies;

/* Player */
typedef struct s_player
{
	t_vec2		pos;
	t_vec2		dir;
	t_vec2		plane;
	int			hp;
	int			max_hp;
	int			ammo;
	int			max_ammo;
	int			score;
	int			keys;
	t_weapon	weapon;
}	t_player;

/* Ray */
typedef struct s_ray
{
	t_vec2	dir;
	t_vec2	delta_dist;
	t_vec2	side_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_wall_dist;
	int		map_x;
	int		map_y;
}	t_ray;

/* Game */
typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_map		*map;
	t_player	*player;
	t_texture	*textures;
	t_hud		*hud;
	t_weapon	*weapon;
	t_enemies	*enemies;
	int			is_running;
	int			game_over;
}	t_game;

/* Core functions */
int		init_game(t_game *game, char *map_file);
void	cleanup_game(t_game *game);
void	game_loop(t_game *game);

/* Parsing functions */
int		parse_cub_file(t_game *game, char *filename);
int		parse_textures(t_game *game, char *line);
int		parse_colors(t_game *game, char *line);
int		parse_map(t_game *game, char **lines);
int		validate_map(t_map *map);
int		find_player_position(t_game *game);
int		is_valid_map_char(char c);

/* Raycasting functions */
void	cast_rays(t_game *game);
void	init_ray(t_ray *ray, t_player *player, int x);
void	perform_dda(t_ray *ray, t_game *game);
double	calculate_distance(t_ray *ray, t_player *player);

/* Rendering functions */
void	render_frame(t_game *game);
void	draw_walls(t_game *game, int x, t_ray *ray);
void	draw_floor_ceiling(t_game *game);
void	draw_minimap(t_game *game);
void	apply_texture(t_game *game, int x, int y, mlx_texture_t *texture);

/* Door functions */
void	init_doors(t_game *game);
void	update_doors(t_game *game);
void	toggle_door(t_game *game, int x, int y);
t_door	*find_door(t_game *game, int x, int y);
int		is_door_position(t_game *game, int x, int y);

/* Controls functions */
void	handle_keyboard(mlx_key_data_t keydata, void *param);
void	handle_continuous_input(t_game *game);
void	handle_mouse(double xpos, double ypos, void *param);
void	handle_mouse_click(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
void	move_player(t_game *game, int direction);
void	rotate_player(t_game *game, double angle);

/* Utils functions */
void	error_exit(char *message);
void	free_split(char **split);
int		is_valid_map_char(char c);
double	normalize_angle(double angle);

/* HUD functions */
int		init_hud(t_game *game);
void	update_hud(t_game *game);
void	render_hud(t_game *game);
void	draw_crosshair(t_game *game);
void	draw_health_bar(t_game *game);
void	draw_ammo_counter(t_game *game);
void	draw_score_display(t_game *game);
void	cleanup_hud(t_game *game);

/* Weapon functions */
int		init_weapon(t_game *game);
void	update_weapon(t_game *game);
void	render_weapon(t_game *game);
void	shoot_weapon(t_game *game);
void	enhanced_shoot_weapon(t_game *game);
void	cast_shot_ray(t_game *game);
void	reload_weapon(t_game *game);
void	cleanup_weapon(t_game *game);

/* Shooting functions */
void	cast_shot_ray(t_game *game);
void	enhanced_shoot_weapon(t_game *game);

/* Enemy functions */
int		init_enemies(t_game *game);
void	update_enemies(t_game *game);
void	render_enemies(t_game *game);
void	spawn_enemy(t_game *game, double x, double y, int type);
void	enemy_ai_update(t_game *game, t_enemy *enemy);
void	enemy_attack(t_game *game, t_enemy *enemy);
int		enemy_can_see_player(t_game *game, t_enemy *enemy);
void	damage_enemy(t_game *game, int enemy_index, int damage);
void	cleanup_enemies(t_game *game);

/* Pathfinding functions (A*) */
t_node	**find_path_astar(t_game *game, int start_x, int start_y, int target_x, int target_y, int *path_length);
void	free_path(t_node **path, int path_length);
t_vec2	get_next_position_on_path(t_node **path, int path_length, int current_step);

/* Game Over functions */
void	render_game_over_screen(t_game *game);
void	handle_game_over(t_game *game);
void	handle_game_over_input(t_game *game, mlx_key_data_t keydata);
void	restart_game(t_game *game);
void	reset_player_position(t_game *game);
void	draw_simple_game_over(t_game *game, int start_x, int start_y);
void	draw_simple_message(t_game *game, int start_x, int start_y, const char *message);

#endif
