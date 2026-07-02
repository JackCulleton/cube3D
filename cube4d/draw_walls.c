#include "header.h"

// map_x = ray_x / TILE_SIZE;
// // 150 / 64 = 2

// tile_start_x = map_x * TILE_SIZE;
// // 2 * 64 = 128

// hit_part_x = ray_x - tile_start_x;
// // 150 - 128 = 22


static int find_wall_x(t_ray *ray)
{
	int tile_start;

	int hit_part_x;

	tile_start = ray->map_x * TILE_SIZE;

	hit_part_x = ray->ray_x - tile_start;

	return(hit_part_x);
}

static int	get_pixel(t_img *tile, int x, int y)
{
	char	*pixel;
	int		bytes_per_pixel;
	int		row_position;
	int		column_position;
	int		final_position;

	if (x < 0 || y < 0)
		return (0);
	if (x >= tile->width || y >= tile->height)
		return (0);

	bytes_per_pixel = tile->bits_per_pixel / 8;

	row_position = y * tile->line_length;
	column_position = x * bytes_per_pixel;
	final_position = row_position + column_position;

	pixel = tile->addr + final_position;

	return (*(int *)pixel);
}

void	draw_walls(t_app *app, t_ray *ray, int screen_x)
{
	double	line_h;
	int		line_start;
	int		line_end;
	int		y;
	int colour;

	int texture_y;
	int tile_x_pos;

	if (ray->distance <= 0)
		return ;

	line_h = (TILE_SIZE * 320) / ray->distance;

	if (line_h > WINDOW_HEIGHT)
		line_h = WINDOW_HEIGHT;

	line_start = (WINDOW_HEIGHT / 2) - (line_h / 2);
	line_end = (WINDOW_HEIGHT / 2) + (line_h / 2);

	y = line_start;

	tile_x_pos = find_wall_x(ray);

	while (y < line_end)
	{
		texture_y = ((y - line_start) * TILE_SIZE) / (line_end - line_start);

		if(ray->wall_direction == NORTH)
		{
		colour = get_pixel(&app-> tile, tile_x_pos, texture_y);
		put_pixel(&app->img, screen_x, y, colour);
		}
		if(ray->wall_direction == SOUTH)
		{
		// colour = get_pixel(&app-> tile, tile_x_pos, texture_y);
		put_pixel(&app->img, screen_x, y,  0x00FFFF);
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
