/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 10:16:13 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/30 20:19:34 by pmalato          ###   ########.fr       */
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

int	thread_setup(t_coder *c_list, size_t id, \
t_dongle *d_list, t_arguments *parsed)
{
	t_thread	*thread;

	thread = new_thread_struct(c_list, d_list, parsed);
	if (!thread)
		return (0);
	if (pthread_create(&c_list[id].thread, NULL, coder_routine, (void *)thread))
	{
		free(thread);
		return (0);
	}
	return (1);
}

long	current_time(void)
{
	struct timeval	c_time;

	gettimeofday(&c_time, NULL);
	return ((c_time.tv_sec * 1000 + c_time.tv_usec / 1000));
}

long	compile(long c_time, t_thread *thread)
{
	thread->coder->deadline = current_time() \
+ thread->parsed->time_to_burnout;
	printf("%ld %d is compiling\n", c_time, thread->coder->id);
	usleep(thread->parsed->time_to_compile * 1000);
	return (current_time() - thread->parsed->clock_start);
}

long	debug_and_refactor(long c_time, t_thread *thread)
{
	long			d_end;

	printf("%ld %d is debugging\n", c_time, thread->coder->id);
	usleep(thread->parsed->time_to_debug * 1000);
	d_end = current_time() - thread->parsed->clock_start;
	printf("%ld %d is refactoring\n", d_end, thread->coder->id);
	usleep(thread->parsed->time_to_refactor * 1000);
	return (current_time() - thread->parsed->clock_start);
}
