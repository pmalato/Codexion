/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:24:29 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/31 10:50:52 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	monitor_cleanup(t_coder *c_list, t_dongle *d_list, size_t i)
{
	coder_mutex_cleanup(c_list, i);
	thread_cleanup(c_list, i);
	free(c_list);
	dongle_mutex_cleanup(d_list, i);
	free(d_list);
}

int	is_program_over(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		pthread_mutex_lock(&c_list[i].mutex);
		if (!c_list[i].done)
		{
			pthread_mutex_unlock(&c_list[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&c_list[i].mutex);
		i++;
	}
	return (1);
}

void	init_coder_mutex(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		pthread_mutex_init(&c_list[i].mutex, NULL);
		i++;
	}
}

void	start_deadlines(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		c_list[i].deadline = c_list->parsed->clock_start + \
c_list->parsed->time_to_burnout;
		i++;
	}
}

int	check_burnout(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		pthread_mutex_lock(&c_list[i].mutex);
		if (c_list[i].deadline <= current_time())
		{
			c_list[i].alive = false;
			printf("%ld %d burned out\n", current_time() - \
c_list->parsed->clock_start, c_list[i].id);
			pthread_mutex_unlock(&c_list[i].mutex);
			return (0);
		}
		pthread_mutex_unlock(&c_list[i].mutex);
		i++;
	}
	return (1);
}
