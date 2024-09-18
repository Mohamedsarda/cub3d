/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:39 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:02:40 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../parsing.h"

void	ft_check_map_print(t_data *data)
{
	ft_putstr_fd("Error: Check The Map Provided\n", 2);
	free_double_arr(data->map);
	free(data->ea);
	free(data->so);
	free(data->we);
	free(data->no);
	exit (1);
}

void	ft_check_color_print(t_data *data)
{
	ft_putstr_fd("Error: Check The Colors Provided\n", 2);
	free_double_arr(data->map);
	exit (1);
}

void	ft_print_cords_err(t_init *init)
{
	ft_putstr_fd("Error: Please Check The Coordinates\n", 2);
	free_double_arr(init->file);
	free_double_arr(init->map);
	exit (1);
}

void	ft_check_file(t_init *init)
{
	if (ft_count_arrgs_in_file(init, "NO") != 1
		|| ft_count_arrgs_in_file(init, "SO") != 1
		|| ft_count_arrgs_in_file(init, "WE") != 1
		|| ft_count_arrgs_in_file(init, "EA") != 1
		|| ft_count_arrgs_in_file(init, "F") != 1
		|| ft_count_arrgs_in_file(init, "C") != 1)
	{
		ft_putstr_fd("Error: Check Your File\n", 2);
		free_double_arr(init->file);
		exit (1);
	}
}
