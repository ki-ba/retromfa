/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edarnand <edarnand@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 22:18:35 by kbarru            #+#    #+#             */
/*   Updated: 2025/10/25 17:11:17 by edarnand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "retromfa.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

static int	sclose(int fd)
{
	if (fd > STDERR_FILENO)
		return (close(fd));
	printf("Warning : refusing to close fd %d.\n", fd);
	return (-1);
}
static int print_hex(const unsigned char *buffer, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		printf("%X	", buffer[i]);
	printf("\n");
	return (size);
}
int	main(int argc, char *argv[])
{
	unsigned char buffer[BUFFER_SIZE];
	unsigned char buf2[BUFFER_SIZE];

	const unsigned char sig1[] = IMG_SIG_1;
	const unsigned char sig2[] = IMG_SIG_2;
	int n_read_bytes = 0;
	if (argc < 2)
	{
		printf("%s\n", USAGE_MSG);
		return (USAGE);
	}
	// void* mlx = mlx_init();
	// void* wind = mlx_new_window(mlx, 1920, 1080, "test");
	// mlx_loop(mlx);

	int file = open(argv[1], O_RDONLY);
	int file_prev = open(argv[1], O_RDONLY);
	if (file_prev < 0 || file < 0)
	{
		sclose(file_prev);
		sclose(file);
		printf("%s %s\n", FILE_ERR_MSG, argv[1]);
		return (1);
	}

	n_read_bytes = read(file, buffer, BUFFER_SIZE); // first 16 bytes are not checked for images
	n_read_bytes = n_read_bytes | read(file_prev, buf2, BUFFER_SIZE);
	printf("looking for :\n");
	print_hex(sig1, 4);
	printf("\n");
	print_hex(sig2, 4);
	while(n_read_bytes > 0)
	{
	n_read_bytes = read(file, buffer, BUFFER_SIZE);
	// print_hex(buffer, BUFFER_SIZE);
	if (memmem(buffer, BUFFER_SIZE, sig1, 4))
		{
			printf("omg picture signature found!!\n");
			print_hex(buffer, BUFFER_SIZE);
			// look for metadata just before (in buf2 if necessary)
			// then read more to get full picture
		}
	else if (memmem(buffer, BUFFER_SIZE, sig2, 4))
		{
			printf("omg picture signature found!!\n");
			print_hex(buffer, BUFFER_SIZE);
		}
	// else
	// 	printf("nothing found here.\n");
	n_read_bytes = n_read_bytes | read(file_prev, buf2, BUFFER_SIZE);
	}
	sclose(file);
	sclose(file_prev);
	// (void)wind;
	return (0);
}
