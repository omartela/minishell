/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 21:54:14 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/08 15:11:31 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_formatted(const char *format, va_list args)
{
	int	check_write;

	check_write = 0;
	if (*format == 'c')
		check_write = ft_print_char(va_arg(args, int));
	else if (*format == 's')
		check_write = ft_print_str(va_arg(args, char *));
	else if (*format == 'p')
		check_write = ft_print_ptr(va_arg(args, void *));
	else if (*format == 'd' || *format == 'i')
		check_write = ft_print_nbr(va_arg(args, int));
	else if (*format == 'u')
		check_write = ft_print_unsnbr(va_arg(args, unsigned int));
	else if (*format == 'x' || *format == 'X')
		check_write = ft_print_hex(va_arg(args, unsigned int), *format);
	else if (*format == '%')
		check_write = write(1, "%", 1);
	return (check_write);
}

static int	ft_helper(const char *format, va_list args)
{
	int		check_write;
	int		counter;

	counter = 0;
	while (*format)
	{
		if (*format == '%')
		{
			check_write = ft_print_formatted(++format, args);
			if (check_write == -1)
				return (-1);
			counter = counter + check_write;
		}
		else
		{
			if (write(1, format, 1) == -1)
				return (-1);
			counter++;
		}
		if (!*format)
			return (0);
		format++;
	}
	return (counter);
}

int	ft_printf(const char *format, ...)
{
	int		counter;
	va_list	args;

	va_start(args, format);
	counter = 0;
	counter = ft_helper(format, args);
	va_end(args);
	if (counter == -1)
		return (-1);
	return (counter);
}
