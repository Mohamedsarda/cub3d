/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:04:03 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:04:04 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../parsing.h"

char	*ft_read(char *line, int fd)
{
	int		byte;
	char	*buffer;

	byte = 1;
	if (!line)
		line = ft_strdup("");
	buffer = ft_calloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (byte && ft_strchr(line, '\n'))
	{
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte == -1)
			return (free(buffer), NULL);
		buffer[byte] = '\0';
		line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (line);
}

char	*ft_line(char *line)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	if (!line[0])
		return (NULL);
	while (line[i] && line[i] != '\n')
		i++;
	ptr = ft_calloc(i + 1);
	if (!ptr)
		return (NULL);
	j = 0;
	while (j < i)
	{
		ptr[j] = line[j];
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}

char	*ft_next(char *line)
{
	char	*ptr;
	int		i;
	int		j;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		i++;
	if (!line[i])
		return (free(line), NULL);
	ptr = ft_calloc(ft_strlen(line) - i + 1);
	if (!ptr)
		return (NULL);
	j = 0;
	while (line[i])
		ptr[j++] = line[i++];
	ptr[j] = '\0';
	free(line);
	return (ptr);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	buffer = ft_read(buffer, fd);
	if (!buffer)
		return (NULL);
	line = ft_line(buffer);
	buffer = ft_next(buffer);
	return (line);
}
