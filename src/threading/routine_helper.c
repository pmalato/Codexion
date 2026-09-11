/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:44:55 by pecoelho          #+#    #+#             */
/*   Updated: 2026/09/11 08:56:06 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	execute_edf(t_thread *thread, t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	if (ft_strcmp(thread->parsed->scheduler, "edf") && \
!d->state && d->cooldown <= current_time())
		edf(thread, d);
	pthread_mutex_unlock(&d->mutex);
}

void	register_wait(t_dongle *d, int id)
{
	pthread_mutex_lock(&d->mutex);
	if (d->queue->queue[0] != id && d->queue->queue[1] != id)
	{
		if (d->queue->queue[0] == -1)
		{
			d->queue->queue[0] = id;
			d->queue->size++;
		}
		else if (d->queue->queue[1] == -1)
		{
			d->queue->queue[1] = id;
			d->queue->size++;
		}
	}
	pthread_mutex_unlock(&d->mutex);
}

int	are_dongles_ready(t_dongle *d1, t_dongle *d2, int id)
{
	if (d1 == d2)
		return (0);
	if (d1->queue->queue[0] != id || d2->queue->queue[0] != id)
		return (0);
	if (d1->state || d2->state)
		return (0);
	if (current_time() < d1->cooldown || current_time() < d2->cooldown)
		return (0);
	return (1);
}

void	announce_taken(t_thread *thread)
{
	safe_print(&thread->parsed->print, "%ld %d has taken a dongle\n", \
current_time() - thread->parsed->clock_start, thread->coder->id + 1);
	safe_print(&thread->parsed->print, "%ld %d has taken a dongle\n", \
current_time() - thread->parsed->clock_start, thread->coder->id + 1);
}
