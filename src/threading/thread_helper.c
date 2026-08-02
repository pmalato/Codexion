/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 09:47:21 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/02 12:36:07 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	thread_cleanup(t_coder *c_list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_join(c_list[i].thread, NULL);
		i++;
	}
}

void	dongle_mutex_cleanup(t_dongle *d_list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(d_list[i].queue);
		pthread_mutex_destroy(&d_list[i].mutex);
		pthread_cond_destroy(&d_list[i].cond);
		i++;
	}
}

void	coder_mutex_cleanup(t_coder *c_list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&c_list[i].mutex);
		i++;
	}
}

void	dongle_broadcast(t_dongle *d_list, t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		pthread_mutex_lock(&d_list[i].mutex);
		pthread_cond_broadcast(&d_list[i].cond);
		pthread_mutex_unlock(&d_list[i].mutex);
		i++;
	}
}

long	get_coder_deadline(t_coder *coder)
{
	long	deadline;
	pthread_mutex_lock(&coder->mutex);
	deadline = coder->deadline;
	pthread_mutex_unlock(&coder->mutex);
	return (deadline);
}
