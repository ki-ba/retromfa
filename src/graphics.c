#include "retromfa.h"
#include "mlx.h"
#include <stdint.h>

static	void fill_color_24bit(t_color *color, const uint8_t *target, uint32_t j)
{
	color->b = target[j + 0];
	color->g = target[j + 1];
	color->r = target[j + 2];
}

static	void fill_color_15bit(t_color *color, const uint8_t *target, uint32_t j)
{
	color->r = ((target[j + 1] & 0b01111100) >> 2);
	color->g = ((target[j + 1] & 0b00000011) << 3) | ((target[j + 0] & 0b11100000) >> 5);
	color->b = ((target[j + 0] & 0b00011111));
}

uint32_t	fill_img(int is_24, t_image *img, const uint8_t *target)
{
	const int		byte_size = is_24 ? 3 : 2;
	const uint32_t	target_size = img->width * img->height * byte_size;

	if (is_24 && img->width % 2)
		img->width++;

	t_color		color;
	uint32_t	j;
	for (j = 0; j < target_size - byte_size - 1; j += byte_size)
	{
		if (is_24)
			fill_color_24bit(&color, target, j);
		else
			fill_color_15bit(&color, target, j);

		if (!is_24)
			color.color *= 8;
		const int pixel = j / byte_size;
		const int x = pixel % img->width;
		const int y = pixel / img->width;
		pixel_put(img, x, y, color.color);
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
