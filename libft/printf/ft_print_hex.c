/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:22:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/08 13:42:44 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_helper_hex(unsigned int n, int *counter, char format)
{
	char	c;

	c = '\0';
	if (n >= 16)
	{
		if (ft_helper_hex(n / 16, counter, format) == -1)
			return (-1);
		if (ft_helper_hex(n % 16, counter, format) == -1)
			return (-1);
	}
	else
	{
		if (n < 10)
			c = n + '0';
		else if (format == 'X')
			c = n - 10 + 'A';
		else if (format == 'x')
			c = n - 10 + 'a';
		if (write(1, &c, 1) == -1)
			return (-1);
		*counter = *counter + 1;
	}
	return (*counter);
}

int	ft_print_hex(unsigned int n, char format)
{
	int	counter;

	counter = 0;
	return (ft_helper_hex(n, &counter, format));
}
