#include "header.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WINDOW_WIDTH || y >= WINDOW_HEIGHT)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static void	draw_square(t_img *img, int x, int y, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < TILE_SIZE)
	{
		i = 0;
		while (i < TILE_SIZE)
		{
			put_pixel(img, x + i, y + j, color);
			i++;
		}
		j++;
	}
}

void	draw_map(t_app *app, t_img *img)
{
	int	x;
	int	y;

	y = 0;
	while (y < app->map.height)
	{
		x = 0;
		while (app->map_visual[y][x])
		{
			if (app->map_visual[y][x] == '1' )
				draw_square(img, x * TILE_SIZE, y * TILE_SIZE, 0x00333333);
			else if (app->map_visual[y][x] == '0' || app->map_visual[y][x] == 'N' 
					|| app->map_visual[y][x] == 'E' || app->map_visual[y][x] == 'S' 
					|| app->map_visual[y][x] == 'W')
				draw_square(img, x * TILE_SIZE, y * TILE_SIZE, 0x00FFFFFF);
			x++;
		}
		y++;
	}
}
