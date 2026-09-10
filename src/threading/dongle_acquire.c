/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquire.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecoelho <pecoelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:17:27 by pecoelho          #+#    #+#             */
/*   Updated: 2026/09/10 16:00:43 by pecoelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

int	acquire_single(t_thread *thread, t_dongle *d)
{
	int	id;

	id = thread->coder->id;
	register_wait(d, id);
	pthread_mutex_lock(&d->mutex);
	while (!check_stop(thread->parsed) && \
(d->state || current_time() <= d->cooldown || d->queue->queue[0] != id))
		pthread_cond_wait(&d->cond, &d->mutex);
	if (check_stop(thread->parsed))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	d->state = true;
	pthread_mutex_unlock(&d->mutex);
	announce_taken(thread);
	return (1);
}

int	acquire_pair(t_thread *thread, t_dongle *d1, t_dongle *d2)
{
	register_wait(d1, thread->coder->id);
	register_wait(d2, thread->coder->id);
	while (!check_stop(thread->parsed))
	{
		execute_edf(thread, d1);
		execute_edf(thread, d2);
		pthread_mutex_lock(&d1->mutex);
		pthread_mutex_lock(&d2->mutex);
		if (are_dongles_ready(d1, d2, thread->coder->id))
		{
			d1->state = true;
			d2->state = true;
			pthread_mutex_unlock(&d2->mutex);
			pthread_mutex_unlock(&d1->mutex);
			announce_taken(thread);
			return (1);
		}
		pthread_mutex_unlock(&d2->mutex);
		pthread_mutex_unlock(&d1->mutex);
		pthread_mutex_lock(&d1->mutex);
		pthread_cond_wait(&d1->cond, &d1->mutex);
		pthread_mutex_unlock(&d1->mutex);
	}
	return (0);
}

int	acquire_dongle_pair(t_thread *thread, t_dongle *d1, t_dongle *d2)
{
	if (d1 == d2)
		return (acquire_single(thread, d1));
	return (acquire_pair(thread, d1, d2));
}
