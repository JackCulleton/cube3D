#include "header.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

static void	draw_player(t_app *app, t_img *img)
{
	int		x;
	int		y;
	int		i;
	double	line_x;
	double	line_y;
	double	dir_x;
	double	dir_y;

	/* draw player square */
	y = -3;
	while (y <= 3)
	{
		x = -3;
		while (x <= 3)
		{
			put_pixel(img,
				(int)app->player_cord->player_x + x,
				(int)app->player_cord->player_y + y,
				0x00FF0000);
			x++;
		}
		y++;
	}

	/* draw direction line */
	dir_x = cos(app->player_cord->player_ang);
	dir_y = sin(app->player_cord->player_ang);

	i = 0;
	while (i < 20)
	{
		line_x = app->player_cord->player_x + dir_x * i;
		line_y = app->player_cord->player_y + dir_y * i;
		put_pixel(img, (int)line_x, (int)line_y, 0x0000FF00);
		i++;
	}
}

static void	clear_image(t_img *img)
{
	int	x;
	int	y;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			put_pixel(img, x, y, 0x00000000);
			x++;
		}
		y++;
	}
}

static void	render_scene(t_app *app)
{
	clear_image(&app->img);
	draw_map(app, &app->img);
	draw_player(app, &app->img);
	cast_all_rays(app);
	mlx_put_image_to_window(app->mlx, app->win, app->img.img, 0, 0);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static int	key_press(int keycode, t_app	*app)
{
	

	// app = (t_app *)param;
	if (keycode == 65307)
	{
		mlx_destroy_window(app->mlx, app->win);
		exit(0);
	}
	if (keycode == 119)
		app->keys.w = 1;
	else if (keycode == 115)
		app->keys.s = 1;
	else if (keycode == 97)
		app->keys.a = 1;
	else if (keycode == 100)
		app->keys.d = 1;
	return (0);
}

static int	key_release(int keycode, t_app	*app)
{
	

	// app = (t_app *)param;
	if (keycode == 119)
		app->keys.w = 0;
	else if (keycode == 115)
		app->keys.s = 0;
	else if (keycode == 97)
		app->keys.a = 0;
	else if (keycode == 100)
		app->keys.d = 0;
	return (0);
}

static int	game_loop(t_app *app)
{
	long	now;
	double	delta_time;
	double	move_speed;
	double	rot_speed;
	double	move_step;
	t_player *p;

	p = app->player_cord;
	now = get_time_ms();
	delta_time = (now - app->last_time) / 1000.0;
	app->last_time = now;

	move_speed = 80.0;
	rot_speed = 2.5;

	move_step = move_speed * delta_time;

	if (app->keys.w)
	{
		p->player_x += cos(p->player_ang) * move_step;
		p->player_y += sin(p->player_ang) * move_step;
	}

	if (app->keys.s)
	{
		p->player_x -= cos(p->player_ang) * move_step;
		p->player_y -= sin(p->player_ang) * move_step;
	}

	if (app->keys.a)
	{
		p->player_ang -= rot_speed * delta_time;
		if (p->player_ang < 0)
			p->player_ang += 2 * PI;
	}

	if (app->keys.d)
	{
		p->player_ang += rot_speed * delta_time;
		if (p->player_ang > 2 * PI)
			p->player_ang -= 2 * PI;
	}

	p->player_dx = cos(p->player_ang);
	p->player_dy = sin(p->player_ang);

	render_scene(app);
	return (0);
}

void	key_input(t_app *app)
{
	mlx_hook(app->win, 2, 1L << 0, key_press, app);
	mlx_hook(app->win, 3, 1L << 1, key_release, app);
	mlx_loop_hook(app->mlx, game_loop, app);
}
