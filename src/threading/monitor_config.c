/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecoelho <pecoelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:13:34 by pmalato           #+#    #+#             */
/*   Updated: 2026/09/08 17:07:50 by pecoelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	edf(t_thread *thread, t_dongle *dongle)
{
	int		temp;

	if (dongle->state)
		return ;
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
			monitor_cleanup(monitor->parsed, monitor->coders, \
monitor->dongles, monitor->parsed->number_of_coders);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

void	end_monitor(t_monitor *monitor, pthread_t monitor_thread)
{
	pthread_join(monitor_thread, NULL);
	free(monitor);
}

bool	check_stop(t_arguments *parsed)
{
	bool	val;

	pthread_mutex_lock(&parsed->main_lock);
	val = parsed->stop;
	pthread_mutex_unlock(&parsed->main_lock);
	return (val);
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
	pthread_mutex_init(&parsed->main_lock, NULL);
	init_coder_mutex(c_list);
	i = 0;
	start_deadlines(c_list);
	while (i < (size_t)parsed->number_of_coders)
	{
		if (!thread_setup(c_list, i, d_list))
		{
			monitor_cleanup(parsed, c_list, d_list, i);
			return (0);
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, (void *)monitor))
		return (1);
	end_monitor(monitor, monitor_thread);
	return (1);
}
