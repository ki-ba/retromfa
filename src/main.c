#include "retromfa.h"
#include "mlx.h"
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <X11/X.h>

size_t	get_file_size(const char filename[])
{

	struct stat	file_stat;
	if (stat(filename, &file_stat) == -1)
		return (0);
	return (file_stat.st_size);
}

void	display_images(t_image imgs[], int img_count, void *mlx, void *wind, int scroll_mod)
{
	static int initial_scroll = 0;

	initial_scroll += scroll_mod;
	int	x_display_offset = PADDING_SIDES;
	int	y_display_offset = PADDING_SIDES + initial_scroll;
	int	highest_y = PADDING_SIDES;

	for (int i = 0; i < img_count; i++)
	{
		if (imgs[i].height > highest_y)
			highest_y = imgs[i].height;
		if (x_display_offset + imgs[i].width >= WIN_WIDTH - PADDING_SIDES)
		{
			y_display_offset += highest_y;
			// highest_y = 0;
			x_display_offset = PADDING_SIDES;
		}
		int offsets[3] = {x_display_offset, y_display_offset, highest_y};
		display_img(mlx, wind, &imgs[i], offsets, scroll_mod);
		x_display_offset += imgs[i].width;
		x_display_offset += PADDING_SIDES / 2;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf(USAGE_MSG);
		return (WRONG_USAGE_EXIT_STATUS);
	}

	int			fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf(FILE_ERR_MSG);
		return (1);
	}

	const size_t	file_size = get_file_size(argv[1]);
	uint8_t			*str = calloc(sizeof(uint8_t), file_size);
	if (str == NULL)
	{
		close(fd);
		printf(MEM_ERR_MSG);
		return (1);
	}
	const size_t	bytes_read = read(fd, str, file_size);
	close(fd);
	if (bytes_read != file_size)
	{
		free(str);
		printf(READ_ERR_MSG);
		return (1);
	}

	void*		mlx = mlx_init();
	void*		wind = mlx_new_window(mlx, WIN_WIDTH, WIND_HEIGHT, "RetroMFA Viewer");
	int			img_index = 0;
	t_image		imgs[1000];

	uint32_t	curr_flag = (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | (str[3]);
	for (size_t i = 7; i < bytes_read; i++)
	{
		const uint8_t	curr_byte = str[i];

		curr_flag = (curr_flag << 8) | curr_byte;
		if ((curr_flag == IMG_SIG_24 || curr_flag == IMG_SIG_15))
		{
			uint8_t		tmp_height = str[i - 5];
			uint8_t		tmp_width = str[i - 7];
			if (tmp_width == 0 || tmp_height == 0)
				continue;

			t_image	*img = imgs + img_index;
			if (build_img(mlx, img, tmp_height, tmp_width) == NULL)
			{
				mlx_loop_end(mlx);
				break ;
			}
			uint32_t img_size = fill_img(curr_flag == IMG_SIG_24, img, str + i + OFFSET);

			i += img_size + OFFSET;
			img_index++;
		}
	}
	display_images(imgs, img_index, mlx, wind, 0);
	printf("%d images found\n", img_index);
	void	*params[2] = {mlx, wind};
	mlx_hook(wind, DestroyNotify, ResizeRedirectMask, &stop_loop, params);
	mlx_hook(wind, KeyPress, KeyPressMask, &key_hook, params);
	mlx_hook(wind, ButtonPress, ButtonPressMask, &scroll_hook, (void *[]){mlx, wind, imgs, &img_index});
	mlx_loop(mlx);
	for (int i = 0; i < img_index; i++)
		mlx_destroy_image(mlx, imgs[i].img);
	mlx_destroy_window(mlx, wind);
	mlx_destroy_display(mlx);
	free(mlx);
	free(str);
	return (0);
}
