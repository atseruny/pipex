/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atseruny <atseruny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:31:11 by atseruny          #+#    #+#             */
/*   Updated: 2025/01/25 15:52:19 by atseruny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cpy;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	cpy = *lst;
	while (cpy -> next != NULL)
		cpy = cpy -> next;
	cpy -> next = new;
}
