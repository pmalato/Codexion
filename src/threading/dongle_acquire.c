/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquire.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:17:27 by pecoelho          #+#    #+#             */
/*   Updated: 2026/09/11 10:02:04 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	solo_edge_case(t_thread *thread, t_dongle *d)
{
	register_wait(d, thread->coder->id);
	while (!check_stop(thread->parsed))
	{
		pthread_mutex_lock(&d->mutex);
		pthread_cond_wait(&d->cond, &d->mutex);
		pthread_mutex_unlock(&d->mutex);
	}
}

void	wake_up_call(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	pthread_cond_wait(&d->cond, &d->mutex);
	pthread_mutex_unlock(&d->mutex);
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
		wake_up_call(d1);
	}
	return (0);
}

int	acquire_dongle_pair(t_thread *thread, t_dongle *d1, t_dongle *d2)
{
	return (acquire_pair(thread, d1, d2));
}
