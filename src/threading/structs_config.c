/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_config.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecoelho <pecoelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 17:02:05 by pmalato           #+#    #+#             */
/*   Updated: 2026/09/07 13:24:48 by pecoelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

t_dongle	new_dongle(void)
{
	t_dongle	dongle;
	t_queue		*queue;

	dongle.state = false;
	dongle.cooldown = 0;
	queue = malloc(sizeof(t_queue));
	if (!queue)
		queue = NULL;
	else
	{
		queue->queue[0] = -1;
		queue->queue[1] = -1;
		queue->size = 0;
	}
	dongle.queue = queue;
	return (dongle);
}

t_dongle	*dongle_list(t_arguments *parsed)
{
	t_dongle	*d_list;
	size_t		i;
	size_t		len;

	i = 0;
	len = parsed->number_of_coders;
	d_list = malloc(sizeof(t_dongle) * len);
	if (!d_list)
		return (NULL);
	while (i < len)
	{
		d_list[i] = new_dongle();
		pthread_mutex_init(&d_list[i].mutex, NULL);
		pthread_cond_init(&d_list[i].cond, NULL);
		i++;
	}
	return (d_list);
}

t_coder	new_coder(void)
{
	t_coder	coder;

	coder.compiled_times = 0;
	coder.last_compile = 0;
	coder.alive = true;
	coder.done = false;
	return (coder);
}

t_thread	*new_thread_struct(t_coder *c_list, t_coder *coder, \
t_dongle *dongle, t_arguments *parsed)
{
	t_thread	*thread;

	thread = malloc(sizeof(t_thread));
	if (!thread)
		return (NULL);
	thread->c_list = c_list;
	thread->coder = coder;
	thread->d_list = dongle;
	thread->parsed = parsed;
	return (thread);
}

t_coder	*coder_list(t_arguments *parsed)
{
	t_coder		*c_list;
	int			i;

	i = 0;
	c_list = malloc(sizeof(t_coder) * parsed->number_of_coders);
	if (!c_list)
		return (NULL);
	while (i < parsed->number_of_coders)
	{
		c_list[i] = new_coder();
		c_list[i].id = i;
		c_list[i].parsed = parsed;
		i++;
	}
	return (c_list);
}
