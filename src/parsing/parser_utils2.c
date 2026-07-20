/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 16:46:43 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/20 17:27:08 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

t_parse_list	*new_node(void *content)
{
	t_parse_list	*new;

	new = malloc(sizeof(t_parse_list));
	if (!content || !new)
		return (NULL);
	new->content = content;
	new->next = NULL;
}
