/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:48 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/08 14:37:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_helper_ptr(unsigned long ptr, int *counter)
{
	char	c;

	if (ptr >= 16)
	{
		if (ft_helper_ptr((ptr / 16), counter) == -1)
			return (-1);
		if (ft_helper_ptr((ptr % 16), counter) == -1)
			return (-1);
	}
	else
	{
		if (ptr < 10)
			c = ptr + '0';
		else
			c = ptr - 10 + 'a';
		if (write(1, &c, 1) == -1)
			return (-1);
		else
			*counter += 1;
	}
	return (*counter);
}

int	ft_print_ptr(void *p)
{
	int				counter;
	unsigned long	ptr;

	counter = 0;
	ptr = (unsigned long)p;
	if (write(1, "0x", 2) == -1)
		return (-1);
	counter += 2;
	ft_helper_ptr(ptr, &counter);
	return (counter);
}
