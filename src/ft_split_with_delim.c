/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_with_delim.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:19:32 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/05 14:45:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**free_split_on_error(char **array, int i)
{
	while (i >= 0)
	{
		free(array[i]);
		i--;
	}
	free(array);
	array = NULL;
	return (array);
}

int	count_splits(char *str, char delimiter)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
			counter = counter + 2;
		else if (i == 0 || str[i - 1] == delimiter)
			counter++;
		i++;
	}
	return (counter);
}

static char	**split_and_store(char **result, char *str, char delimiter, int start)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
		{
			if (i > start)
			{
				result[j++] = ft_substr(str, start, i - start);
				if (!result[j - 1])
					return (free_split_on_error(result, j - 2));
			}
			result[j++] = ft_substr(str, i, 1);
			if (!result[j - 1])
				return (free_split_on_error(result, j - 2));
			start = i + 1;
		}
		i++;
	}
	if (start < ft_strlen(str))
	{
		result[j++] = ft_substr(str, start, ft_strlen(str) - start);
		if (!result[j - 1])
			return (free_split_on_error(result, j - 2));
	}
	result[j] = (NULL);
	return (result);
}

char	**ft_split_with_delim(char *str, char delimiter)
{
	char	**result;
	int		start;
	int		split_count;

	start = 0;
	if (!str)
		return (NULL);
	split_count = count_splits(str, delimiter);
	result = ft_calloc((split_count + 1), sizeof(char *));
	if (!result)
		return (NULL);
	return (split_and_store(result, str, delimiter, start));
}

void	free_split_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int main() {
	char str[] = ",Hello , Worl,d,";
	char str1[] = "Hi, Hive";
	char str2[] = ",";
	char str3[] = "";

	char delimiter = ',';
	char **split_array = ft_split_with_delim(str, delimiter);
	char **split_array1 = ft_split_with_delim(str1, delimiter);
	char **split_array2 = ft_split_with_delim(str2, delimiter);
	char **split_array3 = ft_split_with_delim(str3, delimiter);
	int i = 0;
	while (split_array[i])
	{
		printf("Element %d: {%s}\n", i, split_array[i]);
		i++;
	}
	free_split_array(split_array);
	i = 0;
	while (split_array1[i])
	{
		printf("Element %d: {%s}\n", i, split_array1[i]);
		i++;
	}
	free_split_array(split_array1);
	i = 0;
	while (split_array2[i])
	{
		printf("Element %d: {%s}\n", i, split_array2[i]);
		i++;
	}
	free_split_array(split_array2);
	i = 0;
	while (split_array3[i])
	{
		printf("Element %d: {%s}\n", i, split_array3[i]);
		i++;
	}
	free_split_array(split_array3);
	return (0);
}