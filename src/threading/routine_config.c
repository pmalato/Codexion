/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:58:36 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/11 12:53:51 by pmalato          ###   ########.fr       */
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

int	dongle_acquire(pthread_cond_t *cond, pthread_mutex_t *mutex,
	t_dongle *d, t_thread *thread)
{
	pthread_mutex_lock(mutex);
	if (d->queue->queue[0] != thread->coder->id && \
d->queue->queue[1] != thread->coder->id)
	{
		if (d->queue->queue[0] == -1)
			d->queue->queue[0] = thread->coder->id;
		else if (d->queue->queue[1] == -1)
			d->queue->queue[1] = thread->coder->id;
		d->queue->size++;
	}
	if (ft_strcmp(thread->parsed->scheduler, "edf"))
		edf(thread, d);
	while (!check_stop(thread->parsed) && \
(d->state || current_time() < d->cooldown || \
d->queue->queue[0] != thread->coder->id))
		pthread_cond_wait(cond, mutex);
	if (check_stop(thread->parsed))
	{
		pthread_mutex_unlock(mutex);
		return (0);
	}
	d->state = true;
	pthread_mutex_unlock(mutex);
	printf("%ld %d has taken a dongle\n",
		current_time() - thread->parsed->clock_start, thread->coder->id);
	return (1);
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
	if (n > 1)
		cond_swap(&c_id, &next_id);
	if (!dongle_acquire(&thread->d_list[c_id].cond, &thread->d_list[c_id].mutex, \
&thread->d_list[c_id], thread))
		return (-1);
	if (n > 1 && !dongle_acquire(&thread->d_list[next_id].cond, \
&thread->d_list[next_id].mutex, &thread->d_list[next_id], thread))
	{
		dongle_release(thread, c_id);
		return (-1);
	}
	time_dongle = current_time() - thread->parsed->clock_start;
	compile(time_dongle, thread);
	dongle_release(thread, c_id);
	if (n > 1)
		dongle_release(thread, next_id);
	return (current_time() - thread->parsed->clock_start);
}

void	*coder_routine(void *arg)
{
	t_thread	*thread;
	long		time1;

	thread = (t_thread *)arg;
	while (thread->coder->compiled_times \
< thread->parsed->number_of_compiles_required && \
get_coder_alive(thread->coder) && !check_stop(thread->parsed))
	{
		time1 = dongle_handler(thread);
		if (time1 == -1)
			break ;
		thread->coder->compiled_times++;
		debug_and_refactor(time1, thread);
	}
	pthread_mutex_lock(&thread->coder->mutex);
	thread->coder->done = true;
	pthread_mutex_unlock(&thread->coder->mutex);
	free(thread);
	return (NULL);
}
