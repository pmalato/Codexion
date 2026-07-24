/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 10:19:57 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/24 10:41:52 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_arguments	*stuff;

	stuff = args_parser(ac, av);
	if (!stuff)
		return (0);
	printf("%d\n", stuff->number_of_coders);
	printf("%d\n", stuff->time_to_burnout);
	printf("%d\n", stuff->time_to_burnout);
	printf("%d\n", stuff->time_to_debug);
	printf("%d\n", stuff->time_to_refactor);
	printf("%d\n", stuff->dongle_cooldown);
	printf("%d\n", stuff->number_of_compiles_required);
	printf("%s\n", stuff->scheduler);
	return (0);
}
