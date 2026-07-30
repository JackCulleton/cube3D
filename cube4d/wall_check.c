#include "header.h"

int	hit_wall(t_app *app, t_player *p, double x, double y)
{
	int	map_x;
	int	map_y;
	int	player_map_x;
	int	player_map_y;

	if (x < 0 || y < 0)
		return (1);

	map_x = (int)(x / TILE_SIZE);
	map_y = (int)(y / TILE_SIZE);

	player_map_x = (int)(p->player_x / TILE_SIZE);
	player_map_y = (int)(p->player_y / TILE_SIZE);

	if (map_x >= app->map.width || map_y >= app->map.height)
		return (1);

	if (map_x != player_map_x && map_y != player_map_y)
	{
		if (app->map_visual[player_map_y][map_x] == '1'
			&& app->map_visual[map_y][player_map_x] == '1')
			return (1);
	}

	return (app->map_visual[map_y][map_x] == '1');
}