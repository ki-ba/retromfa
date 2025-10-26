#include "retromfa.h"
#include "mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <X11/X.h>

size_t	get_file_size(const char filename[])
{
	struct stat	file_stat;
	if (stat(filename, &file_stat) == -1)
		return (0);
	return (file_stat.st_size);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf(USAGE_MSG);
		return (WRONG_USAGE_EXIT_STATUS);
	}
	int		fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf(FILE_ERR_MSG);
		return (1);
	}

	uint8_t		*str = calloc(sizeof(uint8_t), get_file_size(argv[1]));
	if (str == NULL)
	{
		close(fd);
		printf(MEM_ERR_MSG);
		return (1);
	}
	void*		mlx = mlx_init();
	void*		wind = mlx_new_window(mlx, WIN_WIDTH, WIND_HEIGHT, "RetroMFA Viewer");
	const int	bytes_read = read(fd, str, get_file_size(argv[1]));
	uint32_t	target_flag_24 = IMG_SIG_24;
	uint32_t	target_flag_15 = IMG_SIG_15;

	int		x_display_offset = 0;
	int		y_display_offset = 0;
	int		highest_y = 0;
	int		img_index = 0;

	uint32_t	curr_flag = (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | (str[3]);

	for (int i = 7; i < bytes_read; i++)
	{
		const uint8_t	curr_byte = str[i];

		curr_flag = (curr_flag << 8) | curr_byte;
		if (curr_flag == target_flag_24 || curr_flag == target_flag_15)
		{
			img_index++;
		}
		uint32_t	img_size = 0;
		if ((curr_flag == target_flag_24 || curr_flag == target_flag_15) && img_index >= 0)
		{
			uint8_t	tmp_height = str[i - 5];
			uint8_t	tmp_width = str[i - 7];
			if (tmp_width == 0 || tmp_height == 0)
				continue;

			t_image img;
			build_img(mlx, &img, tmp_height, tmp_width);

			const uint8_t *target_item = (uint8_t *)str + i + OFFSET;
			if (curr_flag == IMG_SIG_15)
				img_size = fill_img_15bit_color(img, target_item);
			else
				img_size = fill_img_24bit_color(img, target_item);

			if (img.height > highest_y)
				highest_y = img.height;
			if (x_display_offset + img.width >= WIN_WIDTH)
			{
				y_display_offset += highest_y;
				highest_y = 0;
				x_display_offset = 0;
			}
			mlx_put_image_to_window(mlx, wind, img.img, x_display_offset, y_display_offset);
			x_display_offset = x_display_offset + img.width + 30;
			i += img_size + OFFSET;
		}
	}
	printf("%d images found\n", img_index);
	mlx_loop(mlx);
	return (0);
}
