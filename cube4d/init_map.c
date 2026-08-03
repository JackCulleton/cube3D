#include "header.h"

static void	init_size_data(int *max_width, int *current_width, int *height)
{
	*max_width = 0;
	*current_width = 0;
	*height = 0;
}


void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

static t_map	make_size(int max_width, int height)
{
	t_map	size;

	size.width = 0;
	size.height = 0;
	if (max_width <= 0 || height <= 0)
		return (size);
	size.width = max_width;
	size.height = height;
	return (size);
}

static void	update_size(char buffer, int *max_width, int *current_width,
		int *height)
{
	if (buffer == '\n')
	{
		if (*current_width > *max_width)
			*max_width = *current_width;
		*current_width = 0;
		(*height)++;
	}
	else
		(*current_width)++;
}

static void	finalize_last_row(int *max_width, int *current_width, int *height)
{
	if (*current_width > 0)
	{
		if (*current_width > *max_width)
			*max_width = *current_width;
		(*height)++;
	}
}

t_map	read_ber(char *filename)
{
	int		fd;
	char	buffer;
	int		max_width;
	int		current_width;
	int		height;

	init_size_data(&max_width, &current_width, &height);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (make_size(0, 0));
	while (read(fd, &buffer, 1) > 0)
		update_size(buffer, &max_width, &current_width, &height);
	finalize_last_row(&max_width, &current_width, &height);
	close(fd);
	return (make_size(max_width, height));
}

static void	store_char(char **map, int *i, int *j, char buffer)
{
	if (buffer == '\n')
	{
		map[*i][*j] = '\0';
		(*i)++;
		*j = 0;
	}
	else
	{
		map[*i][*j] = buffer;
		(*j)++;
	}
}

static char	**alloc_map_rows(t_map size)
{
	char	**map;
	int		i;

	map = malloc(sizeof(char *) * (size.height + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (i < size.height)
	{
		map[i] = malloc(sizeof(char) * (size.width + 1));
		if (!map[i])
			return (NULL);
		i++;
	}
	map[i] = NULL;
	return (map);
}

static void	finalize_row(char **map, int i, int j, t_map size)
{
	if (j > 0 && i < size.height)
		map[i][j] = '\0';
}

static char	**load_map(char *filename, t_map size)
{
	char	**map;
	char	buffer;
	int		fd;
	int		i;
	int		j;

	map = alloc_map_rows(size);
	if (!map)
		return (NULL);
	i = 0;
	j = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	while (read(fd, &buffer, 1) > 0)
		store_char(map, &i, &j, buffer);
	finalize_row(map, i, j, size);
	close(fd);
	return (map);
}

void	init_map(t_app *app, char *path)
{
	app->map = read_ber(path);
	app->map_visual = load_map((char *)path, app->map);
}