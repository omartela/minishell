/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:17:38 by irychkov          #+#    #+#             */
/*   Updated: 2024/04/29 14:42:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*ptr_haystack;
	size_t	len_needle;
	size_t	steps;
	size_t	i;

	i = 0;
	steps = 0;
	ptr_haystack = (char *)haystack;
	if (!(*needle))
		return (ptr_haystack);
	if (len == 0 || !(*haystack))
		return (NULL);
	len_needle = ft_strlen(needle);
	while (*ptr_haystack && steps <= len - len_needle)
	{
		while (i < len && ptr_haystack[i] == needle[i] && ptr_haystack[i]
			&& needle[i])
			i++;
		if (needle[i] == '\0')
			return (ptr_haystack);
		i = 0;
		ptr_haystack++;
		steps++;
	}
	return (0);
}
