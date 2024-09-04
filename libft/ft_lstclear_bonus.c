/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:17:37 by irychkov          #+#    #+#             */
/*   Updated: 2024/04/24 12:46:58 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*ptr;
	t_list	*tmp;

	if (lst && del)
	{
		ptr = *lst;
		while (ptr != NULL)
		{
			tmp = ptr->next;
			ft_lstdelone(ptr, del);
			ptr = tmp;
		}
		*lst = NULL;
	}
}
