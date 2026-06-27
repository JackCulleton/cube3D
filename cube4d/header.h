#ifndef HEADER_H
# define HEADER_H

# include <stdlib.h>
# include <sys/time.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
# include "minilibx-linux/mlx.h"

#define PI 3.1415926535
#define TILE_SIZE 64
#define RAY_WIDTH 4
#define FOV 1.0471975512
#define WIN_WIDTH 800    


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
}	t_img;

typedef struct s_app
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_player	*player_cord;
	t_keys		keys;
	t_map		map;
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

#endif
