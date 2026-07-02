#include "header.h"

static void	init_player(t_player *player)
{
	player->player_x = 100.0;
	player->player_y = 100.0;
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
	load_image(&app, &app.tile, "New-Project (1).xpm");

	app.win = mlx_new_window(app.mlx, 1024, 512, "cube4d");
	if (!app.win)
		return (1);
	app.img.img = mlx_new_image(app.mlx, 1024, 512);
	if (!app.img.img)
		return (1);
	app.img.addr = mlx_get_data_addr(app.img.img,
			&app.img.bits_per_pixel, &app.img.line_length, &app.img.endian);
	init_player(&player);
	init_keys(&app.keys);
	app.last_time = get_time_ms();
	app.player_cord = &player;
	init_map(&app, argv[1]);
	key_input(&app);
	mlx_loop(app.mlx);
	return (0);
}
