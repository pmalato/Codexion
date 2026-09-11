/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 10:16:13 by pmalato           #+#    #+#             */
/*   Updated: 2026/09/11 08:55:37 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

t_monitor	*new_monitor_struct(t_arguments *parsed, \
t_dongle *d_list, t_coder *c_list)
{
	t_monitor	*monitor;

	monitor = malloc(sizeof(t_monitor));
	if (!monitor)
		return (NULL);
	monitor->parsed = parsed;
	monitor->dongles = d_list;
	monitor->coders = c_list;
	return (monitor);
}

int	thread_setup(t_coder *c_list, size_t id, \
t_dongle *d_list)
{
	t_thread	*thread;
	t_coder		*coder;

	coder = &c_list[id];
	thread = new_thread_struct(c_list, coder, d_list, coder->parsed);
	if (!thread)
		return (0);
	if (pthread_create(&c_list[id].thread, NULL, coder_routine, (void *)thread))
	{
		free(thread);
		return (0);
	}
	return (1);
}

long	current_time(void)
{
	struct timeval	c_time;

	gettimeofday(&c_time, NULL);
	return ((c_time.tv_sec * 1000 + c_time.tv_usec / 1000));
}

void	compile(long c_time, t_thread *thread)
{
	if (check_stop(thread->parsed))
		return ;
	pthread_mutex_lock(&thread->coder->mutex);
	thread->coder->deadline = current_time() \
+ thread->parsed->time_to_burnout;
	pthread_mutex_unlock(&thread->coder->mutex);
	safe_print(&thread->parsed->print, \
"%ld %d is compiling\n", c_time, thread->coder->id + 1);
	usleep(thread->parsed->time_to_compile * 1000);
}

void	debug_and_refactor(long c_time, t_thread *thread)
{
	long			d_end;

	if (check_stop(thread->parsed))
		return ;
	safe_print(&thread->parsed->print, \
"%ld %d is debugging\n", c_time, thread->coder->id + 1);
	usleep(thread->parsed->time_to_debug * 1000);
	d_end = current_time() - thread->parsed->clock_start;
	if (check_stop(thread->parsed))
		return ;
	safe_print(&thread->parsed->print, \
"%ld %d is refactoring\n", d_end, thread->coder->id + 1);
	usleep(thread->parsed->time_to_refactor * 1000);
}
