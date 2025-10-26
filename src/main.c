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

static void	pixel_put(t_image *img, int x, int y, unsigned int color)
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

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void display_image_24(t_image img, const unsigned char *target)
{
	if (img.width % 2)
		img.width++;

	int target_size = img.width * img.height * 3;
	for (int j = 0; j < target_size; j += 3)
	{
		unsigned char b = target[j + 0];
		unsigned char g = target[j + 1];
		unsigned char r = target[j + 2];
		unsigned int color = (r << 16) | (g << 8) | b;

		const int pixel = j / 3;
		const int x = pixel % img.width;
		const int y = pixel / img.width;
		pixel_put(&img, x, y, color);

	}
}

void display_image_16(t_image img, const unsigned char *target)
{
	int target_size = img.width * img.height * 2;

	for (int j = 0; j < target_size; j += 2)
	{
		unsigned char b = 8 *((target[j + 0] & 0b11111000) >> 3);
		unsigned char g = 8 * (((target[j + 0] & 0b00000111) << 2) | ((target[j + 1] & 0b11000000) >> 6));
		unsigned char r = 8 *((target[j + 1] & 0b00111110) >> 1);

		// r = reverse(r);
		// g = reverse(g);
		// b = reverse(b);

		if (r && g && b)
			printf("%hhu %hhu %hhu\n\n\n\n\n", r, g, b);
		unsigned int color = ((r << 16)| (g << 8) | b) ;
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

int	main(void)
{
	void* mlx = mlx_init();
	if (mlx == NULL)
		return (EXIT_FAILURE);
	void* wind = mlx_new_window(mlx, WIN_WIDTH, WIND_HEIGHT, "test");
	if (wind == NULL)
		return (free_mlx(mlx, NULL));
	mlx_hook(wind, DestroyNotify, ResizeRedirectMask, &clean_exit, (void*[]){mlx, wind});
	int fd = open("file/white2.mfa", O_RDWR);
	if (fd == -1)
		return (free_mlx(mlx, wind));
	char str[1500000];
	const int bytes_read = read(fd, str, 1500000);
	close(fd);
	const uint32_t target_flag = 0x06100000;


	int	x_display_offset = 0;
	int	y_display_offset = 0;
	int highest_y = 0;
	int img_index = 0;
	uint32_t curr_flag = (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | (str[3]);
	for (int i = 100; i < bytes_read; i++)
	{
		const unsigned char	curr_byte = str[i];
		const unsigned char	tmp_height = str[i - 5];
		unsigned char	tmp_width = str[i - 7];

		curr_flag = (curr_flag << 8) | curr_byte;
		if (curr_flag == target_flag)
			img_index++;
		if (curr_flag == target_flag && img_index >= 0)
		{
			printf("goal !! img:%d\n", img_index);
			t_image img;
			if (build_img(mlx, &img, tmp_height, tmp_width) == NULL)
			{
				free_mlx(mlx, wind);
				exit(EXIT_FAILURE);
			}

			static int offset = 13;
			const unsigned char *target_item = (unsigned char *)str + i + offset;
			if (target_flag == IMG_SIG_16)
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
				mlx_put_image_to_window(mlx, wind, img.img, x_display_offset, y_display_offset);
				x_display_offset = x_display_offset + img.width + 30;

			}
			else
			{
				mlx_put_image_to_window(mlx, wind, img.img, x_display_offset, y_display_offset);
				x_display_offset = x_display_offset + img.width + 30;
			}
			mlx_destroy_image(mlx, img.img);
			// i += j;
		}
	}
	mlx_loop(mlx);
	return (0);
}
