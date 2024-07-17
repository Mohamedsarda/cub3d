#include "../parsing.h"

int    ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

int    ft_strchr(char *str, int c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (0);
        i++;
    }
    return (1);
}

char    *ft_strdup(char *str)
{
    int     i;
    char    *ptr;

    i = 0;
    if (!str)
        return (NULL);
    ptr = ft_calloc(ft_strlen(str) + 1);
    if (!ptr)
        return (NULL);
    while (str[i])
    {
        ptr[i] = str[i];
        i++;
    }

    ptr[i] = '\0';
    return (ptr);
}

char    *ft_strjoin(char *s1, char *s2)
{
    char    *ptr;
    int     i;
    int     j;


    if (!s1 || !s2)
        return (NULL);
    ptr = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!ptr)
        return NULL;
    i = 0;
    j = 0;
    while (s1[j])
        ptr[i++] = s1[j++];
    j = 0;
    while (s2[j])
        ptr[i++] = s2[j++];
    ptr[i] = '\0';
    free(s1);
    return (ptr);
}

char	*ft_calloc(int count)
{
	char	*str;
	int		i;

	if (count < 1)
		return (NULL);
	str = (char *)malloc(count * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < count)
		str[i++] = 0;
	return (str);
}
