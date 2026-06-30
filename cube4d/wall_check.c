#include "header.h"

int hit_wall(t_app *app, double x, double y)
{
   int	map_x;
	int	map_y;

	if (x < 0 || y < 0)
		return (1);

	map_x = (int)(x / TILE_SIZE);
	map_y = (int)(y / TILE_SIZE);

    if (map_x >= app->map.width || map_y >= app->map.height)
		return (1);

   return (app->map_visual[map_y][map_x] == '1');

}