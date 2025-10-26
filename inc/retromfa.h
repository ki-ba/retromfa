#ifndef RETROMFA_H
# define RETROMFA_H

# define USAGE_MSG "Usage: ./retromfa input_file.mfa\n"
# define FILE_ERR_MSG "Error opening file"
# define BUFFER_SIZE 16
# define USAGE 2
# define IMG_SIG_24 0x04100000
# define IMG_SIG_15 0x06100000
# define WIND_HEIGHT 1080
# define WIN_WIDTH WIND_HEIGHT * (16. / 9.)

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

#endif
