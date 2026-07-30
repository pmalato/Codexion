/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:13:34 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/30 20:18:25 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	*fifo(void)
{
	return (NULL);
}

void	*edf(void)
{
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_coder	*c_list;
	t_coder	*coder;

	c_list = (t_coder *)arg;
	while (1)
	{
		if (!check_burnout(c_list) || is_program_over(c_list))
		{
			if (!check_burnout(c_list))
			{
				coder = check_burned(c_list);
				printf("%d %d burned out", current_time() - \
c_list->parsed->clock_start, coder);
			}
			thread_cleanup(c_list, c_list->parsed->number_of_coders);
			free_coders(c_list, c_list->parsed->number_of_coders);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	monitor_thread(t_arguments *parsed)
{
	pthread_t	monitor;
	t_dongle	*d_list;
	t_coder		*c_list;
	size_t		i;

	d_list = dongle_list(parsed);
	c_list = coder_list(parsed, d_list);
	parsed->clock_start = current_time();
	start_deadlines(parsed, c_list);
	i = 0;
	while (i < parsed->number_of_coders)
	{
		if (!thread_setup(c_list, i, d_list, parsed))
		{
			thread_cleanup(c_list, i);
			free(&c_list[i]);
			return (0);
		}
		i++;
	}
	return (1);
	pthread_create(&monitor, NULL, monitor_routine, (void *)c_list);
}
