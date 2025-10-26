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

int	stop_loop(void *params[])
{
	void	*mlx = params[0];
	void	*wind = params[1];
	(void) wind;

	mlx_loop_end(mlx);
	return (0);
}

int	key_hook(int keycode, void *params[])
{
	void	*mlx = params[0];
	void	*wind = params[1];
	(void) wind;

	if (keycode == 65307)
		mlx_loop_end(mlx);
	return (0);
}

int scroll_hook(int keycode, int x, int y, void *params[])
{
	void	*mlx = params[0];
	void	*wind = params[1];
	t_image	*imgs = params[2];
	int		count = *((int *)params[3]);

	(void) mlx;
	(void) x;
	(void) y;
	(void) wind;

	if(keycode == 4 || keycode == 5)
		mlx_clear_window(mlx, wind);
	if (keycode == 4) // Scroll up
		display_images(imgs, count, mlx, wind, 50);
	else if (keycode == 5) // Scroll down
		display_images(imgs, count, mlx, wind, -50);

	return (0);
}
