/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:13:34 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/02 00:45:00 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	edf(t_thread *thread, t_dongle *dongle)
{
	int		temp;

	if (dongle->queue->queue[0] != -1 && dongle->queue->queue[1] != -1)
	{
		if (get_coder_deadline(&thread->c_list[dongle->queue->queue[0]]) > \
get_coder_deadline(&thread->c_list[dongle->queue->queue[1]]))
		{
			temp = dongle->queue->queue[0];
			dongle->queue->queue[0] = dongle->queue->queue[1];
			dongle->queue->queue[1] = temp;
		}
	}
}

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (1)
	{
		if (!check_burnout(monitor->dongles, monitor->coders) || \
is_program_over(monitor->coders))
		{
			monitor_cleanup(monitor->coders, monitor->dongles, \
monitor->parsed->number_of_coders);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	monitor_thread(t_arguments *parsed, t_dongle *d_list, t_coder *c_list)
{
	pthread_t	monitor_thread;
	t_monitor	*monitor;
	size_t		i;

	monitor = new_monitor_struct(parsed, d_list, c_list);
	if (!monitor)
		return (0);
	parsed->clock_start = current_time();
	init_coder_mutex(c_list);
	i = 0;
	start_deadlines(c_list);
	while (i < (size_t)parsed->number_of_coders)
	{
		if (!thread_setup(c_list, i, d_list))
		{
			monitor_cleanup(c_list, d_list, i);
			free(parsed);
			return (0);
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, (void *)monitor))
		return (1);
	pthread_join(monitor_thread, NULL);
	return (1);
}
