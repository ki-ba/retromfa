#include "retromfa.h"
#include "mlx.h"
#include <stdint.h>

uint32_t	fill_img_24bit_color(t_image *img, const uint8_t *target)
{
	if (img->width % 2)
		img->width++;

	uint32_t	target_size = img->width * img->height * 3;
	uint32_t	j;

	for (j = 0; j < target_size - 2; j += 3)
	{
		uint8_t b = target[j + 0];
		uint8_t g = target[j + 1];
		uint8_t r = target[j + 2];
		uint32_t color = (r << 16) | (g << 8) | b;

		const int pixel = j / 3;
		const int x = pixel % img->width;
		const int y = pixel / img->width;
		pixel_put(img, x, y, color);
	}
	return (j);
}

uint32_t	fill_img_15bit_color(t_image *img, const uint8_t *target)
{
	uint32_t	target_size = img->width * img->height * 2;
	uint32_t	j;

	for (j = 0; j < target_size - 1; j += 2)
	{
		const uint8_t r = ((target[j + 1] & 0b01111100) >> 2);
		const uint8_t g = ((target[j + 1] & 0b00000011) << 3) | ((target[j + 0] & 0b11100000) >> 5);
		const uint8_t b = ((target[j + 0] & 0b00011111));

		uint32_t color = 8 * ((r << 16)| (g << 8) | b) ;
		const int pixel = j / 2;
		const int x = pixel % img->width;
		const int y = pixel / img->width;
		pixel_put(img, x, y, color);
	}
	return (j);
}

uint32_t	fill_img(int is_24, t_image *img, const uint8_t *target)
{
	const int		byte_size = is_24 ? 3 : 2;
	const uint32_t	target_size = img->width * img->height * byte_size;

	if (is_24 && img->width % 2)
		img->width++;

	uint8_t		r, g, b;
	uint32_t	color;
	uint32_t	j;
	for (j = 0; j < target_size - byte_size - 1; j += byte_size)
	{
		if (is_24)
		{
			b = target[j + 0];
			g = target[j + 1];
			r = target[j + 2];
		}
		else
		{
			r = ((target[j + 1] & 0b01111100) >> 2);
			g = ((target[j + 1] & 0b00000011) << 3) | ((target[j + 0] & 0b11100000) >> 5);
			b = ((target[j + 0] & 0b00011111));
		}
		color = (r << 16) | (g << 8) | b;
		if (!is_24)
			color *= 8;

		const int pixel = j / byte_size;
		const int x = pixel % img->width;
		const int y = pixel / img->width;
		pixel_put(img, x, y, color);
	}
	return (j);
}

void	display_img(void *mlx, void *wind, t_image *img, int offsets[3], int scroll_mod)
{

	int x_display_offset = offsets[0];
	int y_display_offset = offsets[1];

	// y_display_offset += scroll_mod;
	(void)scroll_mod;

		mlx_put_image_to_window(mlx, wind, img->img, x_display_offset, y_display_offset);
}
