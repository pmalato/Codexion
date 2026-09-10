/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecoelho <pecoelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:44:00 by pecoelho          #+#    #+#             */
/*   Updated: 2026/09/10 19:58:49 by pecoelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	cond_swap(int *a, int *b)
{
	if (*a > *b)
	{
		*a ^= *b;
		*b ^= *a;
		*a ^= *b;
	}
}

void	dongle_release(t_thread *thread, size_t id)
{
	pthread_mutex_lock(&thread->d_list[id].mutex);
	thread->d_list[id].queue->queue[0] = thread->d_list[id].queue->queue[1];
	thread->d_list[id].queue->queue[1] = -1;
	thread->d_list[id].queue->size--;
	thread->d_list[id].state = false;
	thread->d_list[id].cooldown = current_time() + \
thread->parsed->dongle_cooldown;
	pthread_cond_broadcast(&thread->d_list[id].cond);
	pthread_mutex_unlock(&thread->d_list[id].mutex);
}

long	dongle_handler(t_thread *thread)
{
	long	time_dongle;
	int		c_id;
	int		next_id;
	int		n;

	n = thread->parsed->number_of_coders;
	c_id = thread->coder->id;
	next_id = (c_id + 1) % thread->parsed->number_of_coders;
	cond_swap(&c_id, &next_id);
	if (c_id == next_id)
		solo_edge_case(thread, &thread->d_list[c_id]);
	if (!acquire_dongle_pair(thread, &thread->d_list[c_id], \
&thread->d_list[next_id]))
		return (-1);
	time_dongle = current_time() - thread->parsed->clock_start;
	compile(time_dongle, thread);
	dongle_release(thread, c_id);
	dongle_release(thread, next_id);
	return (current_time() - thread->parsed->clock_start);
}

void	*coder_routine(void *arg)
{
	t_thread		*thread;
	pthread_mutex_t	print;
	long			time;

	thread = (t_thread *)arg;
	pthread_mutex_init(&print, NULL);
	while (thread->coder->compiled_times \
< thread->parsed->number_of_compiles_required && \
get_coder_alive(thread->coder) && !check_stop(thread->parsed))
	{
		time = dongle_handler(thread);
		if (time == -1 || check_stop(thread->parsed))
			break ;
		thread->coder->compiled_times++;
		debug_and_refactor(time, thread);
	}
	pthread_mutex_lock(&thread->coder->mutex);
	thread->coder->done = true;
	pthread_mutex_unlock(&thread->coder->mutex);
	pthread_mutex_destroy(&print);
	free(thread);
	return (NULL);
}
