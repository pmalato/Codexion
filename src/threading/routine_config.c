/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:58:36 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/02 00:55:19 by pmalato          ###   ########.fr       */
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

void	dongle_acquire(pthread_cond_t *cond, pthread_mutex_t *mutex,
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
	while (d->state || current_time() < d->cooldown || \
d->queue->queue[0] != thread->coder->id)
		pthread_cond_wait(cond, mutex);
	d->state = true;
	pthread_mutex_unlock(&d->mutex);
	printf("%ld %d has taken dongle %d\n",
		current_time() - thread->parsed->clock_start, thread->coder->id,
	d->id);
}

void	dongle_release(t_thread *thread, size_t id)
{
	pthread_mutex_lock(&thread->d_list[id].mutex);
	thread->d_list[id].queue->queue[0] = thread->d_list[id].queue->queue[1];
	thread->d_list[id].queue->queue[1] = -1;
	thread->d_list[id].queue->size--;
	thread->d_list[id].state = false;
	thread->d_list[id].cooldown = current_time() + thread->parsed->dongle_cooldown;
	pthread_cond_broadcast(&thread->d_list[id].cond);
	pthread_mutex_unlock(&thread->d_list[id].mutex);
}

long	dongle_handler(t_thread *thread)
{
	long	time_dongle;
	int		c_id;
	int		next_id;

	c_id = thread->coder->id;
	next_id = (c_id + 1) % thread->parsed->number_of_coders;
	cond_swap(&c_id, &next_id);
	dongle_acquire(&thread->d_list[c_id].cond, &thread->d_list[c_id].mutex,
		&thread->d_list[c_id], thread);
	dongle_acquire(&thread->d_list[next_id].cond,
		&thread->d_list[next_id].mutex, &thread->d_list[next_id], thread);
	time_dongle = current_time() - thread->parsed->clock_start;
	compile(time_dongle, thread);
	dongle_release(thread, c_id);
	dongle_release(thread, next_id);
	return (current_time() - thread->parsed->clock_start);
}

void	*coder_routine(void *arg)
{
	t_thread	*thread;
	long		time1;
	long		time2;

	thread = (t_thread *)arg;
	while (thread->coder->compiled_times \
< thread->parsed->number_of_compiles_required && thread->coder->alive)
	{
		time1 = dongle_handler(thread);
		thread->coder->compiled_times++;
		time2 = debug_and_refactor(time1, thread);
	}
	printf("%ld %d is finished\n", time2, thread->coder->id);
	pthread_mutex_lock(&thread->coder->mutex);
	thread->coder->done = true;
	pthread_mutex_unlock(&thread->coder->mutex);
	free(thread);
	return (NULL);
}
