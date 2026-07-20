/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 11:04:22 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/20 17:33:07 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

t_parse_list	**get_value_list(int ac, char **av)
{
	t_parse_list	**list;
	t_parse_list	*node;
	int				num_value;
	int				i;

	if (!av || !is_nmb_of_args_valid(ac))
		return (NULL);
	i = 0;
	while (av[i] <= 7)
	{
		num_value = is_numeric_arg_valid(av[i]);
		if (!num_value)
			return (NULL);
		else
		{
			node->content = num_value;
			node->next = node;
		}
		i++;
	}
}
