/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 21:55:08 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/03 11:07:48 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int argument)
{
	if (argument > 96 && argument < 123)
		return (argument - 32);
	else
		return (argument);
}
