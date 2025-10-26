#include <stdlib.h>
#include "mlx.h"

int free_mlx(void *mlx, void *wind)
{
	if (wind != NULL && mlx != NULL)
		mlx_destroy_window(mlx, wind);
	if (mlx != NULL)
	{
		mlx_destroy_display(mlx);
		free(mlx);
	}
	return (1);
}

int	clean_exit(void *args[])
{
	void *mlx = args[0];
	void *wind = args[1];
	mlx_loop_end(mlx);
	free_mlx(mlx, wind);
	return (0);
}
