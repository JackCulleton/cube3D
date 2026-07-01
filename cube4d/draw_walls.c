#include "header.h"

void	draw_walls(t_app *app, t_ray *ray, int screen_x)
{
	double	line_h;
	int		line_start;
	int		line_end;
	int		y;

	if (ray->distance <= 0)
		return ;
	line_h = (TILE_SIZE * 320) / ray->distance;
	if (line_h > WINDOW_HEIGHT)
		line_h = WINDOW_HEIGHT;
	line_start = (WINDOW_HEIGHT / 2) - (line_h / 2);
	line_end = (WINDOW_HEIGHT / 2) + (line_h / 2);
	y = line_start;
	while (y < line_end)
	{
		if(ray->wall_direction == NORTH)
		{
		put_pixel(&app->img, screen_x, y, 0x00FF0000);
		}
		if(ray->wall_direction == SOUTH)
		{
		put_pixel(&app->img, screen_x, y, 0x00FFFF00);
		}
		if(ray->wall_direction == EAST)
		{
		put_pixel(&app->img, screen_x, y, 0x00FF00FF);
		}
		if(ray->wall_direction == WEST)
		{
		put_pixel(&app->img, screen_x, y, 0x00FFFFFF);
		}
		
		y++;
	}
}
