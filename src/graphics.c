#include "retromfa.h"

uint32_t	fill_img_24bit_color(t_image img, const uint8_t *target)
{
	if (img.width % 2)
		img.width++;

	uint32_t	target_size = img.width * img.height * 3;
	uint32_t	j;

	for (j = 0; j < target_size - 2; j += 3)
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
	return (j);
}

uint32_t	fill_img_15bit_color(t_image img, const uint8_t *target)
{
	uint32_t	target_size = img.width * img.height * 2;
	uint32_t	j;

	for (j = 0; j < target_size - 1; j += 2)
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
	return (j);
}
