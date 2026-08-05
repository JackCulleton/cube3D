#include "header.h"

static void	init_player(t_player *player, t_app	*app)
{

	int	y;
int	x;

y = 0;
while (app->map_visual[y] != NULL)
{
	x = 0;
	while (app->map_visual[y][x] != '\0')
	{
		if (app->map_visual[y][x] == 'N')
		{
			player->player_x = x * TILE_SIZE + TILE_SIZE / 2;
			player->player_y = y * TILE_SIZE + TILE_SIZE / 2;
			player->player_ang = 3 * PI / 2;
			return ;
		}
			if (app->map_visual[y][x] == 'S')
		{
			player->player_x = x * TILE_SIZE + TILE_SIZE / 2;
			player->player_y = y * TILE_SIZE + TILE_SIZE / 2;
			player->player_ang = PI / 2;
			return ;
		}
			if (app->map_visual[y][x] == 'E')
		{
			player->player_x = x * TILE_SIZE + TILE_SIZE / 2;
			player->player_y = y * TILE_SIZE + TILE_SIZE / 2;
			player->player_ang = 0;
			return ;
		}
			if (app->map_visual[y][x] == 'W')
		{
			player->player_x = x * TILE_SIZE + TILE_SIZE / 2;
			player->player_y = y * TILE_SIZE + TILE_SIZE / 2;
			player->player_ang = PI;
			return ;
		}
		x++;
	}
	y++;
}
	// player->player_x = 100.0;
	// player->player_y = 100.0;
}

static void	init_keys(t_keys *keys)
{
	keys->w = 0;
	keys->a = 0;
	keys->s = 0;
	keys->d = 0;
}



int	main(int argc, char **argv)
{
	t_app	app;
	t_player	player;

    if(argc != 2)
    {
        return 0;
    }

	app.mlx = mlx_init();
	if (!app.mlx)
		return (1);
	load_image(&app, &app.north_texture, "North.xpm");
	load_image(&app, &app.east_texture, "East.xpm");
	load_image(&app, &app.south_texture, "South.xpm");
	load_image(&app, &app.west_texture, "West.xpm");

	app.win = mlx_new_window(app.mlx, 1024, 512, "cube4d");
	if (!app.win)
		return (1);
	app.img.img = mlx_new_image(app.mlx, 1024, 512);
	if (!app.img.img)
		return (1);
	app.img.addr = mlx_get_data_addr(app.img.img,
			&app.img.bits_per_pixel, &app.img.line_length, &app.img.endian);
	// init_player(&player, &app);
	init_keys(&app.keys);
	app.last_time = get_time_ms();
	app.player_cord = &player;
	init_map(&app, argv[1]);
	init_player(&player, &app);
	key_input(&app);
	mlx_loop(app.mlx);


	return (0);
}
