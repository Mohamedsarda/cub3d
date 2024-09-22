/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:41 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:02:42 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../parsing.h"

int	ft_count_arrgs_in_file(t_init *init, char *key)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	count = 0;
	while (init->file[i])
	{
		j = 0;
		while (init->file[i][j] && (init->file[i][j] == ' '
				|| init->file[i][j] == '\t'))
			j++;
		if (ft_strncmp(init->file[i] + j, key, ft_strlen(key)) == 0)
			count++;
		i++;
	}
	return (count);
}

void	ft_load_cords(t_data *data, char *cur, int i)
{
	if (i == 0)
		data->no = cur;
	else if (i == 1)
		data->so = cur;
	else if (i == 2)
		data->we = cur;
	else if (i == 3)
		data->ea = cur;
	else
		free(cur);
}

int	ft_check_2_cords_init(char **cords, t_data *data)
{
	int		i;
	int		j;
	char	**tmp;

	i = -1;
	while (cords[++i])
	{
		tmp = ft_split(cords[i], ' ', '\t');
		if (!tmp)
			return (-1);
		j = 0;
		while (tmp[j])
			j++;
		if (j != 2)
			return (free_double_arr(tmp), -1);
		(ft_load_cords(data, ft_strdup(tmp[1]), i), free_double_arr(tmp));
	}
	return (1);
}
