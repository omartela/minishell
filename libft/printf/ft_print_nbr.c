/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:51:01 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/08 14:40:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_helper_nbr(int n, int *counter)
{
	char	c;

	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	else if (n < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		*counter = *counter + 1;
		n = -n;
	}
	if (n >= 10)
	{
		if (ft_helper_nbr(n / 10, counter) == -1)
			return (-1);
	}
	c = n % 10 + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	*counter = *counter + 1;
	return (*counter);
}

int	ft_print_nbr(int n)
{
	int		counter;

	counter = 0;
	return (ft_helper_nbr(n, &counter));
}
