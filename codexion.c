/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 10:19:57 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/31 12:07:34 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_arguments	*stuff;
	t_dongle	*d_list;
	t_coder		*c_list;

	stuff = args_parser(ac, av);
	if (!stuff)
		return (0);
	d_list = dongle_list(stuff);
	if (!d_list)
	{
		free(stuff);
		return (0);
	}
	c_list = coder_list(stuff);
	if (!c_list)
	{
		free(stuff);
		free(d_list);
		return (0);
	}
	if (!monitor_thread(stuff, d_list, c_list))
		return (0);
	free(stuff);
	return (0);
}
