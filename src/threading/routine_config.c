/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:58:36 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/31 12:26:03 by pmalato          ###   ########.fr       */
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
	while (d->state || current_time() < d->cooldown)
		pthread_cond_wait(cond, mutex);
	d->state = true;
	printf("%ld %d has taken a dongle\n",
		current_time() - thread->parsed->clock_start, thread->coder->id);
}

void	dongle_unlock(pthread_cond_t *c1, pthread_cond_t *c2,\
	pthread_mutex_t *m1, pthread_mutex_t *m2)
{
	pthread_cond_broadcast(c1);
	pthread_mutex_unlock(m1);
	pthread_cond_broadcast(c2);
	pthread_mutex_unlock(m2);
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
	thread->d_list[c_id].state = false;
	thread->d_list[next_id].state = false;
	thread->d_list[c_id].cooldown = (current_time() + \
thread->parsed->dongle_cooldown);
	thread->d_list[next_id].cooldown = (current_time() + \
thread->parsed->dongle_cooldown);
	dongle_unlock(&thread->d_list[c_id].cond, &thread->d_list[next_id].cond,
		&thread->d_list[c_id].mutex, &thread->d_list[next_id].mutex);
	return (current_time() - thread->parsed->clock_start);
}

void	*coder_routine(void *arg)
{
	t_thread	*thread;
	long		time1;
	long		time2;

	thread = (t_thread *)arg;
	while (thread->coder->compiled_times \
< thread->parsed->number_of_compiles_required)
	{
		time1 = dongle_handler(thread);
		thread->coder->compiled_times++;
		time2 = debug_and_refactor(time1, thread);
	}
	printf("%ld %d is finished\n", time2, thread->coder->id);
	return (NULL);
}
