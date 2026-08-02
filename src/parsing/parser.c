/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 23:01:17 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/02 12:51:10 by pmalato          ###   ########.fr       */
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
		if (i == 1 && is_numeric_arg_valid(av[1]) && !codex_atoi(av[1]))
			return (NULL);
		if (!is_numeric_arg_valid(av[i]))
			return (NULL);
		i++;
	}
	if (!is_fifo_or_edf(av[8]))
		return (NULL);
	parsed = malloc(sizeof(t_arguments));
	if (!parsed)
		return (NULL);
	parsed->stop = false;
	pack_values(parsed, av);
	return (parsed);
}

void	request_stop(t_arguments *parsed)
{
	pthread_mutex_lock(&parsed->main_lock);
	parsed->stop = true;
	pthread_mutex_unlock(&parsed->main_lock);
}
