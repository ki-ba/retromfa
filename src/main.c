#include "retromfa.h"
#include "mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <X11/X.h>

static void	pixel_put(t_image *img, int x, int y, const uint32_t color)
{
	char	*dst;

	dst = img->addr + (y * img->len_line + x * img->bpp);
	*(unsigned int *)dst = color;
}

void	*build_img(void *mlx, t_image *img, int height, int width)
{
	img->height = height;
	img->width = width;
	img->img = mlx_new_image(mlx, img->width, img->height);
	if (img->img == NULL)
		return (NULL);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->len_line, &img->endian);
	img->bpp /= 8;
	return (img->img);
}

void display_image_24(t_image img, const uint8_t *target)
{
	if (img.width % 2)
		img.width++;

	int target_size = img.width * img.height * 3;
	for (int j = 0; j < target_size; j += 3)
	{
		uint8_t b = target[j + 0];
		uint8_t g = target[j + 1];
		uint8_t r = target[j + 2];
		uint32_t color = (r << 16) | (g << 8) | b;

		const int pixel = j / 3;
		const int x = pixel % img.width;
		const int y = pixel / img.width;
		pixel_put(&img, x, y, color);
	}
}

void display_image_16(t_image img, const uint8_t *target)
{
	int target_size = img.width * img.height * 2;

	for (int j = 0; j < target_size; j += 2)
	{
		const uint8_t r = ((target[j + 1] & 0b01111100) >> 2);
		const uint8_t g = ((target[j + 1] & 0b00000011) << 3) | ((target[j + 0] & 0b11100000) >> 5);
		const uint8_t b = ((target[j + 0] & 0b00011111));

		uint32_t color = 8 * ((r << 16)| (g << 8) | b) ;
		const int pixel = j / 2;
		const int x = pixel % img.width;
		const int y = pixel / img.width;
		pixel_put(&img, x, y, color);
	}
}
int free_mlx(void *mlx, void *wind)
{
	if (wind != NULL && mlx != NULL)
		mlx_destroy_window(mlx, wind);
	if (mlx != NULL)
	{
		mlx_destroy_display(mlx);
		free(mlx);
	}
	return (1);
}

int	clean_exit(void *args[])
{
	void *mlx = args[0];
	void *wind = args[1];
	mlx_loop_end(mlx);
	free_mlx(mlx, wind);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("%s", USAGE_MSG);
		return (1);
	}
	void*		mlx = mlx_init();
	void*		wind = mlx_new_window(mlx, WIN_WIDTH, WIND_HEIGHT, "test");
	int			fd = open(argv[1], O_RDWR);
	uint8_t		str[1500000];
	const int	bytes_read = read(fd, str, 1500000);
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
		const uint8_t	tmp_height = str[i - 5];
		const uint8_t	tmp_width = str[i - 7];

		curr_flag = (curr_flag << 8) | curr_byte;
		if (curr_flag == target_flag_24 || curr_flag == target_flag_15)
			img_index++;
		if ((curr_flag == target_flag_24 || curr_flag == target_flag_15) && img_index >= 0)
		{
			if (tmp_width <= 0 || tmp_height <= 0)
				continue;
			printf("goal !! img:%d\n", img_index);
			printf("size %d * %d\n", tmp_width, tmp_height);

			t_image img;
			build_img(mlx, &img, tmp_height, tmp_width);

			const int offset = 13;
			const uint8_t *target_item = (uint8_t *)str + i + offset;
			if (curr_flag == IMG_SIG_15)
				display_image_16(img, target_item);
			else
				display_image_24(img, target_item);

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
			// i += j;
		}
	}
	mlx_loop(mlx);
	return (0);
}
