/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 23:01:17 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/22 23:41:55 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	pack_values(t_arguments *parsed, char **av)
{
	parsed->number_of_coders = codex_atoi(av[1]);
	parsed->time_to_burnout = codex_atoi(av[2]);
	parsed->time_to_compile = codex_atoi(av[3]);
	parsed->time_to_debug = codex_atoi(av[4]);
	parsed->time_to_refactor = codex_atoi(av[5]);
	parsed->number_of_compiles_required = codex_atoi(av[6]);
	parsed->dongle_cooldown = codex_atoi(av[7]);
	parsed->scheduler = av[8];
}

t_arguments	*args_parser(int ac, char **av)
{
	t_arguments	*parsed;
	int			i;

	if (!is_nmb_of_args_valid(ac))
		return (NULL);
	i = 1;
	while (i <= 7)
	{
		if (!is_numeric_arg_valid(av[i]))
			return (NULL);
		i++;
	}
	if (!is_fifo_or_edf(av[8]))
		return (NULL);
	parsed = malloc(sizeof(t_arguments));
	if (!parsed)
		return (NULL);
	pack_values(parsed, av);
	return (parsed);
}
