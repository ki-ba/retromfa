#include "retromfa.h"
#include "mlx.h"
#include <stddef.h>

void	pixel_put(t_image *img, int x, int y, const uint32_t color)
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

