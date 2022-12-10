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
			d0+=step;
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
		tmp = d0;
		d0 = d1;
		d1 = tmp;
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

t_point	*get_3d_point_on_canvas(t_3d_point *point, int d)
{
	t_point	*point_2d;

	point_2d = malloc(sizeof(t_point *));
	point_2d->x = (point->x * d) / point->z;
	point_2d->y = (point->y * d) / point->z;
	return (point_2d);
}

int main()
{
	void		*mlx;
	void		*window;
	t_img_data	img_data;

	mlx = mlx_init();
	window = mlx_new_window(mlx, 1600, 800, "Hello World");
	img_data.img = mlx_new_image(mlx, 1600, 800);
	img_data.addr = mlx_get_data_addr(img_data.img,
					&img_data.bits_per_pixel, &img_data.line_length,
					&img_data.endian);
	// t_point	*p1 = new_point(0, 0);
	// t_point	*p2 = new_point(800, 400);

	t_3d_point	v1;
	t_3d_point	v2;
	t_3d_point	v3;
	t_3d_point	v4;
	v1.x = 200;
	v1.y = 200;
	v1.z = 4;

	v2.x = 200;
	v2.y = 50;
	v2.z = 4;

	v3.x = 50;
	v3.y = 50;
	v3.z = 4;

	v4.x = 50;
	v4.y = 200;
	v4.z = 4;

	t_point	*p1 = get_3d_point_on_canvas(&v1, 10);
	t_point	*p2 = get_3d_point_on_canvas(&v2, 10);
	t_point	*p3 = get_3d_point_on_canvas(&v3, 10);
	t_point	*p4 = get_3d_point_on_canvas(&v4, 10);

	t_3d_point	v5;
	t_3d_point	v6;
	t_3d_point	v7;
	t_3d_point	v8;

	v5.x = 200;
	v5.y = 200;
	v5.z = 8;
	
	v6.x = 200;
	v6.y = 50;
	v6.z = 8;

	v7.x = 50;
	v7.y = 50;
	v7.z = 8;

	v8.x = 50;
	v8.y = 200;
	v8.z = 8;

	t_point	*p5 = get_3d_point_on_canvas(&v5, 10);
	t_point	*p6 = get_3d_point_on_canvas(&v6, 10);
	t_point	*p7 = get_3d_point_on_canvas(&v7, 10);
	t_point	*p8 = get_3d_point_on_canvas(&v8, 10);

	mlx_draw_line(img_data, p1, p2);
	mlx_draw_line(img_data, p2, p3);
	mlx_draw_line(img_data, p3, p4);
	mlx_draw_line(img_data, p4, p1);

	mlx_draw_line(img_data, p5, p6);
	mlx_draw_line(img_data, p6, p7);
	mlx_draw_line(img_data, p7, p8);
	mlx_draw_line(img_data, p8, p5);

	mlx_draw_line(img_data, p1, p5);
	mlx_draw_line(img_data, p2, p6);
	mlx_draw_line(img_data, p3, p7);
	mlx_draw_line(img_data, p4, p8);
	// printf("(%d, %d)", p1->x, p1->y);
	mlx_put_image_to_window(mlx, window, img_data.img, 0, 0);
	mlx_loop(mlx);
}
