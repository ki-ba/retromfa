#include "retromfa.h"
#include "mlx.h"
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define WIND_HEIGHT 1080
#define WIN_WIDTH WIND_HEIGHT * (16. / 9.)

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		len_line;
	int		endian;
	int		width;
	int		height;
}	t_image;

static void	pixel_put(t_image *img, int x, int y, unsigned int color)
{
	char	*dst;

	dst = img->addr + (y * img->len_line + x * img->bpp);
	*(unsigned int *)dst = color;
}

void	build_img(void *mlx, t_image *img, int height, int width)
{
	img->height = height;
	img->width = width;
	img->img = mlx_new_image(mlx, img->width, img->height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->len_line, &img->endian);
	img->bpp /= 8;
}

//static int	sclose(int fd)
//{
//	if (fd > STDERR_FILENO)
//		return (close(fd));
//	printf("Warning : refusing to close fd %d.\n", fd);
//	return (-1);
//}
//static int print_hex(const unsigned char *buffer, size_t size)
//{
//	for (size_t i = 0; i < size; ++i)
//		printf("%X	", buffer[i]);
//	printf("\n");
//	return (size);
//}
//int	main(int argc, char *argv[])
//{
//	unsigned char buffer[BUFFER_SIZE];
//	unsigned char buf2[BUFFER_SIZE];

//	const unsigned char sig1[] = IMG_SIG_1;
//	const unsigned char sig2[] = IMG_SIG_2;
//	int n_read_bytes = 0;
//	if (argc < 2)
//	{
//		printf("%s\n", USAGE_MSG);
//		return (USAGE);
//	}
//	// void* mlx = mlx_init();
//	// void* wind = mlx_new_window(mlx, 1920, 1080, "test");
//	// mlx_loop(mlx);

//	int file = open(argv[1], O_RDONLY);
//	int file_prev = open(argv[1], O_RDONLY);
//	if (file_prev < 0 || file < 0)
//	{
//		sclose(file_prev);
//		sclose(file);
//		printf("%s %s\n", FILE_ERR_MSG, argv[1]);
//		return (1);
//	}

//	n_read_bytes = read(file, buffer, BUFFER_SIZE); // first 16 bytes are not checked for images
//	n_read_bytes = n_read_bytes | read(file_prev, buf2, BUFFER_SIZE);
//	printf("looking for :\n");
//	print_hex(sig1, 4);
//	printf("\n");
//	print_hex(sig2, 4);
//	while(n_read_bytes > 0)
//	{
//	n_read_bytes = read(file, buffer, BUFFER_SIZE);
//	// print_hex(buffer, BUFFER_SIZE);
//	if (memmem(buffer, BUFFER_SIZE, sig1, 4))
//		{
//			printf("omg picture signature found!!\n");
//			print_hex(buffer, BUFFER_SIZE);
//			// look for metadata just before (in buf2 if necessary)
//			// then read more to get full picture
//		}
//	else if (memmem(buffer, BUFFER_SIZE, sig2, 4))
//		{
//			printf("omg picture signature found!!\n");
//			print_hex(buffer, BUFFER_SIZE);
//		}
//	// else
//	// 	printf("nothing found here.\n");
//	n_read_bytes = n_read_bytes | read(file_prev, buf2, BUFFER_SIZE);
//	}
//	sclose(file);
//	sclose(file_prev);
//	// (void)wind;
//}

int main(void)
{
	void* mlx = mlx_init();
	void* wind = mlx_new_window(mlx, WIN_WIDTH, WIND_HEIGHT, "test");
	(void)wind;
	int fd = open("file/white1.mfa", O_RDWR);
	char str[1147483];
	const int bytes_read = read(fd, str, 1147483);
	const uint32_t target_flag = 0x04100000;//TODO same thing but with 0x06100000 and a color of 15bit


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
			if (tmp_width % 2)
				tmp_width++;
			build_img(mlx, &img, tmp_height, tmp_width);

			const int bytes_size = img.height * img.width * 3;
			static int offset = 13;
			const unsigned char *target_item = (unsigned char *)str + i + offset;
			int j;
			for (j = 0; j < bytes_size; j += 3)
			{
				unsigned char b = target_item[j + 0];
				unsigned char g = target_item[j + 1];
				unsigned char r = target_item[j + 2];
				unsigned int color = (r << 16) | (g << 8) | b;

				const int pixel = j / 3;
				const int x = pixel % img.width;
				const int y = pixel / img.width;
				pixel_put(&img, x, y, color);

			}
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
			i += j;
		}
	}
	mlx_loop(mlx);
	return (0);
}