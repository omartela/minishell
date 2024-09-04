/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 11:07:08 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/03 11:07:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int argument)
{
	if (argument > 64 && argument < 91)
		return (argument + 32);
	else
		return (argument);
}
