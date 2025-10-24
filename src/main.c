/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 22:18:35 by kbarru            #+#    #+#             */
/*   Updated: 2025/10/24 22:21:33 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s file_1.mfa ... file_n.mfa\n", argv[0]);
		return (1);
	}
	return (0);
}
