/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:58:27 by mknoll           ###   ########.fr       */
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

/* Color components for texture processing */
typedef struct s_color_components
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
}	t_color_components;

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
}	t_texture;

/* Map */
typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	t_color		floor_color;
	t_color		ceiling_color;
	t_texture	textures;
}	t_map;

/* Player */
typedef struct s_player
{
	t_vec2		pos;
	t_vec2		dir;
	t_vec2		plane;
	int			keys;
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
	int			is_running;
	int			game_over;
}	t_game;

/* Structure to hold map parsing parameters */
typedef struct s_map_params
{
	int	start;
	int	height;
	int	width;
}	t_map_params;

/**
 * Structure to hold direction and camera plane vectors
 */
typedef struct s_direction_data
{
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_direction_data;

/**
 * Structure to map texture identifiers to their storage locations
 */
typedef struct s_texture_map
{
	char			*identifier;
	mlx_texture_t	**texture_ptr;
}	t_texture_map;

typedef struct s_wall_draw_params
{
	int				x;
	int				line_height;
	int				draw_start;
	int				draw_end;
	mlx_texture_t	*texture;
}	t_wall_draw_params;

/* Core functions */
int		init_game(t_game *game, char *map_file);
void	cleanup_game(t_game *game);
void	game_loop(t_game *game);
void	on_close(void *param);
int		error_exit_init(t_game *game, char *msg);

/* Parsing functions */
int		parse_cub_file(t_game *game, char *filename);
int		parse_textures(t_game *game, char *line);
int		parse_colors(t_game *game, char *line);
int		parse_map(t_game *game, char **lines);
int		validate_map(t_map *map);
int		find_player_position(t_game *game);
int		is_valid_map_char(char c);

/* File utility functions */
char	**read_file_lines(int fd);
int		validate_cub_extension(char *filename);
char	**open_and_read_file(char *filename);

/* Line utility functions */
char	**resize_lines_array(char **lines, int *capacity, int count);
void	process_line(char *line);
void	free_lines(char **lines);

/* Map parsing utility functions */
int		find_map_start(char **lines);
int		get_max_line_width(char **lines, int start, int count);
int		count_map_lines(char **lines, int map_start);

/* Map validation utility functions */
int		allocate_map_grid(t_game *game, int height, int width);
void	copy_and_pad_line(char *dest, char *src, int max_width);
int		validate_map_line(char *line, int width);

/* Player parsing utility functions */
void	set_north_direction(t_player *player);
void	set_south_direction(t_player *player);
void	set_east_direction(t_player *player);
void	set_west_direction(t_player *player);
void	set_player_direction(t_player *player, char dir);

/* Raycasting functions */
void	cast_rays(t_game *game);
void	init_ray(t_ray *ray, t_player *player, int x);
void	perform_dda(t_ray *ray, t_game *game);
double	calculate_distance(t_ray *ray, t_player *player);

/* Rendering functions */
void	render_frame(t_game *game);
void	draw_walls(t_game *game, int x, t_ray *ray);
void	draw_colored_wall(t_game *game, t_ray *ray, t_wall_draw_params *params);
void	draw_textured_wall(t_game *game, t_ray *ray,
			t_wall_draw_params *params);
void	draw_floor_ceiling(t_game *game);
void	draw_minimap(t_game *game);
void	apply_texture(t_game *game, int x, int y, mlx_texture_t *texture);

/* Controls functions */
void	handle_keyboard(mlx_key_data_t keydata, void *param);
void	handle_continuous_input(t_game *game);
void	handle_mouse(double xpos, double ypos, void *param);
void	handle_mouse_click(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
void	rotate_player(t_game *game, double angle);

/* Movement utility functions */
int		check_collision(t_game *game, double x, double y);
int		check_collision_with_buffer(t_game *game, double x, double y);
t_vec2	calculate_new_position(t_game *game, int direction);
void	move_player(t_game *game, int direction);

/* Utils functions */
void	error_exit(char *message);
void	free_split(char **split);
int		is_valid_map_char(char c);
double	normalize_angle(double angle);
void	clean_textures(t_game *game);
void	clean_map(t_game *game);

/* Game Over functions */
void	render_game_over_screen(t_game *game);
void	handle_game_over(t_game *game);
void	handle_game_over_input(t_game *game, mlx_key_data_t keydata);
void	restart_game(t_game *game);
void	reset_player_position(t_game *game);
void	draw_simple_game_over(t_game *game, int start_x, int start_y);
void	draw_simple_message(t_game *game, int start_x,
			int start_y, const char *message);

#endif
