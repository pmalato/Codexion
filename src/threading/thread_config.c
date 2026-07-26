/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 10:16:13 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/26 11:45:20 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	*coder_routine(void *arg)
{
	t_thread	*thread;

	thread = (t_thread *)arg;
	printf("%p\n", thread->coder);
	printf("%p\n", thread->d_list);
	printf("%p\n", thread->parsed);
	return (NULL);
}

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

size_t	c_list_len(t_coder *arr)
{
	size_t	i;

	i = 0;
	while (arr[i].id)
		i++;
	return (i);
}
