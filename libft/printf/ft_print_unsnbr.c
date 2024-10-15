/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsnbr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:50:10 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/08 14:34:54 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_helper_unsnbr(unsigned int n, int *counter)
{
	char	c;

	if (n >= 10)
	{
		if (ft_helper_unsnbr(n / 10, counter) == -1)
			return (-1);
	}
	c = n % 10 + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	*counter = *counter + 1;
	return (*counter);
}

int	ft_print_unsnbr(unsigned int n)
{
	int		counter;

	counter = 0;
	return (ft_helper_unsnbr(n, &counter));
}
