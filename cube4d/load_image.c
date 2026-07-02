#include "header.h"

void load_image(t_app *app, t_img *tex, char *path)
{
    tex->img = mlx_xpm_file_to_image(app->mlx, path,
			&tex->width, &tex->height);
	if (!tex->img)
	{
		printf("failed to load texture: %s\n", path);
		exit(1);
	}

    	tex->addr = mlx_get_data_addr(tex->img,
			&tex->bits_per_pixel,
			&tex->line_length,
			&tex->endian);
}