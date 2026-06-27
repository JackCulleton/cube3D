#include "header.h"
void	cast_all_rays(t_app *app)
{
	t_ray	ray;
	double	ray_angle;
	int		x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		ray_angle = app->player_cord->player_ang - (FOV / 2)
			+ ((double)x / WIN_WIDTH) * FOV;

		ray_caster(&ray, app, app->player_cord, ray_angle);

		x = x + 10;
	}
}

void	ray_caster(t_ray *ray, t_app *app, t_player *player, double ray_angle)
{
	ray->ray_x = player->player_x;
	ray->ray_y = player->player_y;

	ray->ray_dx = cos(ray_angle);
	ray->ray_dy = sin(ray_angle);

	while (1)
	{
		ray->ray_x += ray->ray_dx;
		ray->ray_y += ray->ray_dy;

		ray->map_x = (int)(ray->ray_x / TILE_SIZE);
		ray->map_y = (int)(ray->ray_y / TILE_SIZE);

		put_pixel(&app->img, (int)ray->ray_x, (int)ray->ray_y, 0x00FF0000);

		if (app->map_visual[(int)ray->map_y][(int)ray->map_x] == '1')
			break;
	}
}
