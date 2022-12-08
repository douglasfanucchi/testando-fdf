#include <mlx.h>
#include <math.h>
#include <stdio.h>
#include "libft/libft.h"
#include "fdf.h"

typedef struct  s_img_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}  t_img_data;

void	mlx_put_pixel_into_image(t_img_data *img_data, int x, int y, int color)
{
	char	*dst;
	int		bytes_per_pixel;

	bytes_per_pixel = img_data->bits_per_pixel / 8;
	dst = img_data->addr + (y * img_data->line_length) + (x * bytes_per_pixel);
	*(unsigned int*)dst = color;
}

t_point	*new_point(int x, int y)
{
	t_point	*point;

	point = malloc(sizeof(t_point *));
	point->x = x;
	point->y = y;
	return (point);
}

t_list	**mlx_linear_interpolate_low(t_list **nodes, int i0, int d0, int i1, int d1)
{
	int	dd;
	int	di;
	int	step;
	int	e;

	dd = d1 - d0;
	di = i1 - i0;
	step = 1;
	e = 0;
	if (dd < 0)
	{
		step = -1;
		dd *= -1;
	}
	ft_lstadd_back(nodes, ft_lstnew(new_point(i0, d0)));
	while (i0 <= i1)
	{
		i0++;
		e += 2*dd;
		if (e >= 2*di)
		{
			d0++;
			e -= 2*di;
		}
		ft_lstadd_back(nodes, ft_lstnew(new_point(i0, d0)));
	}
	return (nodes);
}

t_list	**mlx_linear_interpolate_high(t_list **nodes, int i0, int d0, int i1, int d1)
{
	int	dd;
	int	di;
	int	step;
	int	e;

	dd = d1 - d0;
	di = i1 - i0;
	step = 1;
	e = 0;
	if (dd < 0)
	{
		step = -1;
		dd *= -1;
	}
	ft_lstadd_back(nodes, ft_lstnew(new_point(d0, i0)));
	while (d0 != d1)
	{
		d0+=step;
		e += 2*di;
		if (e >= 2*dd)
		{
			i0++;
			e -= 2*dd;
		}
		ft_lstadd_back(nodes, ft_lstnew(new_point(i0, d0)));
	}
	return (nodes);
}

t_list	**mlx_linear_interpolate(int i0, int d0, int i1, int d1)
{
	int		dd;
	int		di;
	int		tmp;
	t_list	**nodes;

	nodes = malloc(sizeof(t_list **));
	*nodes = NULL;
	if (i0 > i1)
	{
		tmp = i0;
		i0 = i1;
		i1 = tmp;
	}
	di = i1 - i0;
	dd = d1 - d0;
	if (abs(dd) > abs(di))
		return mlx_linear_interpolate_high(nodes, i0, d0, i1, d1);
	return mlx_linear_interpolate_low(nodes, i0, d0, i1, d1);
}

void	del_point(void *point)
{
	free(point);
}

void	mlx_draw_line(t_img_data img_data, t_point *p1, t_point *p2)
{
	t_list	**nodes;
	t_list	*node;
	t_point	*point;

	nodes = mlx_linear_interpolate(p1->x, p1->y, p2->x, p2->y);
	if (!nodes)
		return;
	node = *nodes;
	while (node)
	{
		point = node->content;
		mlx_put_pixel_into_image(&img_data, point->x, point->y, 0x00FF00FF);
		node = node->next;
	}
	ft_lstclear(nodes, del_point);
}

int main()
{
	void		*mlx;
	void		*window;
	t_img_data	img_data;

	mlx = mlx_init();
	window = mlx_new_window(mlx, 800, 400, "Hello World");
	img_data.img = mlx_new_image(mlx, 800, 400);
	img_data.addr = mlx_get_data_addr(img_data.img,
					&img_data.bits_per_pixel, &img_data.line_length,
					&img_data.endian);
	t_point	*p1 = new_point(0, 0);
	t_point	*p2 = new_point(800, 400);
	mlx_draw_line(img_data, p1, p2);
	mlx_put_image_to_window(mlx, window, img_data.img, 0, 0);
	mlx_loop(mlx);
}
