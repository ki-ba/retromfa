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
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	void* mlx = mlx_init();
	void* wind = mlx_new_window(mlx, 1920, 1080, "test");
	mlx_loop(mlx);

	(void)wind;
	return (0);
}
