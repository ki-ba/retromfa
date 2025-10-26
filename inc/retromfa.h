/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retromfa.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 22:21:43 by kbarru            #+#    #+#             */
/*   Updated: 2025/10/24 22:25:08 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETROMFA_H
# define RETROMFA_H

# define USAGE_MSG "Usage: %s file_1.mfa ... file_n.mfa\n"
# define FILE_ERR_MSG "Error opening file"
# define BUFFER_SIZE 16
# define USAGE 2
# define IMG_SIG_24 0x04100000
# define IMG_SIG_16 0x06100000
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
