#include "../parsing.h"
int	ft_words_counter(char *str, char c, char p)
{
	int	i;
	int	words;
	int	is_word;

	i = 0;
	words = 0;
	is_word = 0;
	while (str[i])
	{
		if (str[i] == c || str[i] == p)
			is_word = 0;
		else if (!is_word)
		{
			words++;
			is_word = 1;
		}
		i++;
	}
	return (words);
}

static int	ft_word_len(char *str, char c, char p)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] != c && str[i] != p))
		i++;
	return (i);
}

static void	*ft_free(char **arr, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(arr[j]);
		j++;
	}
	free(arr);
	return (NULL);
}

static char	**ft_putword(char *str, char c, char p, char **dst)
{
	int	i;
	int	len;
	int	j;
	int	words;

	i = 0;
	len = 0;
    words = ft_words_counter(str, c, p);
	while (i < words)
	{
		while (*str == c || *str == p)
			str++;
		len = ft_word_len(str, c, p);
		dst[i] = (char *)ft_calloc(len * sizeof(char) + 1);
		if (!dst[i])
			return (ft_free(dst, i));
		j = 0;
		while (j < len)
			dst[i][j++] = *str++;
		dst[i][j] = '\0';
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

char	**ft_split(char *s, char c, char p)
{
	char	**dst;
	int		words;

	if (!s)
		return (NULL);
	while (*s == ' ' || *s == p)
		s++;
	if (*s == '\0')
		return (NULL);
	words = ft_words_counter(s, c, p);
	dst = (char **)ft_calloc((words + 1) * sizeof(char *));
	if (!dst)
		return (NULL);
	dst = ft_putword(s, c, p, dst);
	return (dst);
}
