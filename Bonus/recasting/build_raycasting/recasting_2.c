#include "../../../recasting.h"

void	ft_load_doors(t_cub *cube, int i, char *path)
{
	cube->doors[i] = mlx_load_png(path);
	if (!cube->doors[i])
		ft_error();
	cube->door_img[i] = mlx_texture_to_image(cube->mlx, cube->doors[i]);
	if (!cube->door_img[i])
		ft_error();
}

static char	*zero(void)
{
	char	*arr;

	arr = malloc(2 * sizeof(char));
	if (!arr)
		return (NULL);
	if (arr == NULL)
		return (NULL);
	arr[0] = '0';
	arr[1] = '\0';
	return (arr);
}

static int	ft_intlen(int n)
{
	int	nbdg;
	int	tmp;

	nbdg = 0;
	tmp = n;
	while (tmp != 0)
	{
		tmp /= 10;
		nbdg++;
	}
	return (nbdg);
}

static void	fillarr(char *res, int n, int nbrdg, int isnegative)
{
	int	index;
	int	digit;

	index = nbrdg - 1;
	while (n != 0)
	{
		if (n < 0)
			digit = -(n % 10);
		else
			digit = n % 10;
		res[index] = digit + '0';
		n /= 10;
		index--;
	}
	if (isnegative != 0)
		res[0] = '-';
	res[nbrdg] = '\0';
}

char	*ft_itoa(int n)
{
	int		nbrdg;
	int		isnegative;
	char	*res;

	if (n == 0)
		return (zero());
	nbrdg = ft_intlen(n);
	isnegative = 0;
	if (n < 0)
	{
		isnegative = 1;
		nbrdg++;
	}
	res = malloc((nbrdg + 1) * sizeof(char));
	if (!res)
		return (NULL);
	fillarr(res, n, nbrdg, isnegative);
	return (res);
}
