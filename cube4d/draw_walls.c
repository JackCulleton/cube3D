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

static int find_wall_y(t_ray *ray)
{
	int tile_start;

	int hit_part_y;

	tile_start = ray->map_y * TILE_SIZE;

	hit_part_y = ray->ray_y - tile_start;

	return(hit_part_y);
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
	int tile_y_pos;

	int draw_start;
	int draw_end;

	if (ray->distance <= 0)
		return ;

	line_h = (TILE_SIZE * 320) / ray->distance;

	// if (line_h > WINDOW_HEIGHT)
	// 	line_h = WINDOW_HEIGHT;

	line_start = (WINDOW_HEIGHT / 2) - (line_h / 2);
	line_end = (WINDOW_HEIGHT / 2) + (line_h / 2);

	draw_start = line_start;
	draw_end = line_end;

	if (draw_start < 0)
		draw_start = 0;
	if (draw_end > WINDOW_HEIGHT)
		draw_end = WINDOW_HEIGHT;

y = draw_start;

	tile_x_pos = find_wall_x(ray);
	tile_y_pos = find_wall_y(ray);

	while (y < draw_end)
	{
		// texture_y = ((y - line_start) * TILE_SIZE) / (line_end - line_start);
		texture_y = ((y - line_start) * app->tile.height) / line_h;

		if(ray->wall_direction == NORTH)
		{
			colour = get_pixel(&app->tile, tile_x_pos, texture_y);
			put_pixel(&app->img, screen_x, y, colour);
		}

		if(ray->wall_direction == SOUTH)
		{
			colour = get_pixel(&app->tile,
				app->tile.width - 1 - tile_x_pos,
				texture_y);
			put_pixel(&app->img, screen_x, y, colour);
		}

		if(ray->wall_direction == EAST)
		{
			colour = get_pixel(&app->tile,
				app->tile.width - 1 - tile_y_pos,
				texture_y);
			put_pixel(&app->img, screen_x, y, colour);
		}

		if(ray->wall_direction == WEST)
		{
			colour = get_pixel(&app->tile, tile_y_pos, texture_y);
			put_pixel(&app->img, screen_x, y, colour);
		}
		
		y++;
	}
}
