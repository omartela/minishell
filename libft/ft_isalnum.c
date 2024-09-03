/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:45:42 by irychkov          #+#    #+#             */
/*   Updated: 2024/05/03 10:15:12 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int argument)
{
	if ((argument > 64 && argument < 91) || (argument > 96 && argument < 123))
		return (1);
	else if (argument > 47 && argument < 58)
		return (1);
	else
		return (0);
}
