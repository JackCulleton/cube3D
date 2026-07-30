#ifndef HEADER_H
# define HEADER_H

# include <stdlib.h>
# include <sys/time.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include <stdio.h>
// # include "minilibx-linux/mlx.h"
#include "mlx.h"

#define PI 3.1415926535
# define TILE_SIZE 64
# define RAY_WIDTH 4
# define FOV 1.0471975512
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 512

typedef enum e_direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}	t_direction;

typedef struct s_player_cord
{
	double	player_x;
	double	player_y;
	double	player_dx;
	double	player_dy;
	double	player_ang;
}	t_player;

typedef struct s_ray
{
	double	ray_dx;
	double ray_dy;
	double ray_x;
	double ray_y;
	int map_x;
	int map_y;
	double distance;
	t_direction wall_direction;
}	t_ray;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
}	t_keys;

typedef struct s_map
{
	int	width;
	int	height;
}	t_map;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int 	height;
	int 	width;
}	t_img;

typedef struct s_app
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		tile;
	t_player	*player_cord;
	t_keys		keys;
	t_map		map;
	t_ray		ray;
	long		last_time;
	char		**map_visual;
}	t_app;

void	key_input(t_app *app);
long	get_time_ms(void);
t_map	read_ber(char *filename);
void	init_map(t_app *app, char *path);
void	draw_map(t_app *app, t_img *img);
void	put_pixel(t_img *img, int x, int y, int color);
void	ray_caster(t_ray *ray, t_app *app, t_player *player, double ray_angle);
void	cast_all_rays(t_app *app);
void	draw_walls(t_app *app, t_ray *ray, int screen_x);
int	hit_wall(t_app *app, t_player *p, double x, double y);
void load_image(t_app *app, t_img *tex, char *path);

void	draw_roof(t_app *app, t_ray *ray, int screen_x);
void	draw_floor(t_app *app, t_ray *ray, int screen_x);
#endif
