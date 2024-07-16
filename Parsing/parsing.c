#include "parsing.h"

void    ff()
{
    system("leaks a.out");
}

void	free_double_arr(char **tmp)
{
	int	i;

	i = 0;
	if (!tmp || !*tmp)
		return ;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (n == 0)
		return (0);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((i < n - 1) && (str1[i] && str2[i]) && (str1[i] == str2[i]))
		i++;
	return (str1[i] - str2[i]);
}

int ft_count_arrgs_int_file(t_init *data, char *key)
{
    int i;
    int j;
    int count;

    i = 0;
    count = 0;
    while (data->file[i])
    {
        j = 0;
        while (data->file[i][j] && (data->file[i][j] == ' ' || data->file[i][j] == '\t'))
            j++;
        if (ft_strncmp(data->file[i] + j, key, ft_strlen(key)) == 0)
            count++;
        i++;
    }
    return (count);
}

int ft_get_start_pos(t_init *data, char *c)
{
    int i;
    int j;

    i = 0;
    while (data->file[i])
    {
        j = 0;
        while (data->file[i][j] && (data->file[i][j] == ' ' || data->file[i][j] == '\t'))
            j++;
        if (ft_strncmp(data->file[i] + j, c, ft_strlen(c)) == 0)
            return (i);
        i++;
    }
    return (-1);
}

void    ft_putstr_fd(char *str, int fd)
{
    int i;

    i = 0;
    while (str[i])
        write (fd, &str[i++], 1);
}

void    ft_check_file(t_init *data)
{
    int i;

    i = 0;
    if (ft_count_arrgs_int_file(data, "NO") != 1
        || ft_count_arrgs_int_file(data, "SO") != 1
        || ft_count_arrgs_int_file(data, "WE") != 1
        || ft_count_arrgs_int_file(data, "EA") != 1
        || ft_count_arrgs_int_file(data, "F") != 1
        || ft_count_arrgs_int_file(data, "C") != 1
        )
    {
        ft_putstr_fd("Error: Please Check Your Map\n", 2);
        free_double_arr(data->file);
        free(data);
        exit (1);
    }
}

int ft_get_coordinats(t_init *data)
{
    int no;
    int so;
    int we;
    int ea;

    no = ft_get_start_pos(data, "NO");
    so = ft_get_start_pos(data, "SO");
    we = ft_get_start_pos(data, "WE");
    ea = ft_get_start_pos(data, "EA");
    if (no == -1 || so == -1 || we == -1 || ea == -1)
        return (-1);
    // i need to check if if there is only one NO and one SO ... #done
    data->coordinats = (char **)ft_calloc(5 * sizeof(char *));
    if (!data->coordinats)
        return (-1);
    data->coordinats[0] = ft_strdup(data->file[no]);
    data->coordinats[1] = ft_strdup(data->file[so]);
    data->coordinats[2] = ft_strdup(data->file[we]);
    data->coordinats[3] = ft_strdup(data->file[ea]);
    data->coordinats[4] = NULL;
    return (1);
}

int ft_get_colors(t_init *data)
{
    int f;
    int c;

    f = ft_get_start_pos(data, "F");
    c = ft_get_start_pos(data, "C");
    if (c == -1 || f == -1)
        return (-1);
    // i need to count if F is only 1 or if C is only one ..... #done
    data->colors = (char **)ft_calloc(3 * sizeof(char *));
    if (!data->colors)
        return (-1);
    data->colors[0] = ft_strdup(data->file[f]);
    data->colors[1] = ft_strdup(data->file[c]);
    data->colors[2] = NULL;
    return (1);
}

int ft_get_map(t_init *data)
{
    int i;
    int j;

    ft_check_file(data);
    i = ft_get_start_pos(data, "1");
    if (i == -1)
        return (-1);
    j = 0;
    data->map = (char **)ft_calloc((data->file_lines - i + 1) * sizeof(char *));
    if (!data->map)
        return (-1);
    while (data->file[i])
        data->map[j++] = ft_strdup(data->file[i++]);
    data->map[j] = NULL;
    ft_get_colors(data);
    ft_get_coordinats(data);
    return (1);
}

int main()
{
    // atexit(ff);
    int fd = open("./Maps/map_1.cub", O_RDONLY);
    char *tmp;

    if (fd == -1 || fd == -1)
        return (1);
    t_init *data;

    data = (t_init *)ft_calloc(sizeof(t_init));
    if (!data)
        return (1);
    int i = 0;
    while (1)
    {
        tmp = get_next_line(fd);
        if (!tmp)
            break ;
        free(tmp);
        i++;
    }
    data->file_lines = i;
    close(fd);
    // printf("%d | \n", i);
    fd = open("./Maps/map_1.cub", O_RDONLY);
    data->file = (char **)ft_calloc((i + 1) * sizeof(t_init *));
    i = 0;
    while (1)
    {
        data->file[i] = get_next_line(fd);
        if (!data->file[i])
            break ;
        i++;
    }
    close(fd);
    ft_get_map(data);

    //
    i = 0;
    while (data->map && data->map[i])
    {
        printf("%s", data->map[i++]);
    }
    i = 0;
    puts("\n|\n");
    while (data->colors && data->colors[i])
    {
        printf("%s", data->colors[i++]);
    }
    i = 0;
    puts("|\n");
    while (data->coordinats && data->coordinats[i])
    {
        printf("%s", data->coordinats[i++]);
    }
    //clean
    // free_double_arr(data->file);
}
