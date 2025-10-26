#ifndef RETROMFA_H
# define RETROMFA_H

# define USAGE_MSG "Usage: ./retromfa input_file.mfa\n"
# define FILE_ERR_MSG "Error opening file\n"
# define READ_ERR_MSG "Error reading file\n"
# define MEM_ERR_MSG "Memory allocation error\n"
# define WRONG_USAGE_EXIT_STATUS 2
# define IMG_SIG_24 0x04100000
# define IMG_SIG_15 0x06100000
# define WIND_HEIGHT 1080
# define WIN_WIDTH WIND_HEIGHT * (16. / 9.)
# define OFFSET 13

# define PADDING_SIDES 50
#include <stdint.h>
#include <stddef.h>

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		len_line;
	int		endian;
	int		width;
	int		height;
	int		scroll_offset;
}	t_image;

typedef union u_color
{
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
	uint32_t	color;
}	t_color;

// PROTOTYPES

// ERROR.C

int		free_mlx(void *mlx, void *wind);
int		clean_exit(void *args[]);

// MAIN.C
size_t	get_file_size(const char filename[]);
void	*build_img(void *mlx, t_image *img, int height, int width);
void	pixel_put(t_image *img, int x, int y, uint32_t color);
void	display_images(t_image imgs[], int img_count, void *mlx, void *wind, int scroll_mod);

// GRAPHICS.C
uint32_t	fill_img(int is_24, t_image *img, const uint8_t *target);
void		display_img(void *mlx, void *wind, t_image *img, int offsets[3], int scroll_mod);

// MLX_UTILS.C
int	stop_loop(void *params[]);
int	key_hook(int keycode, void *params[]);
int scroll_hook(int keycode, int x, int y, void *params[]);

#endif
