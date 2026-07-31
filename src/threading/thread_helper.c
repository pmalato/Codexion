/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 09:47:21 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/31 09:52:07 by pmalato          ###   ########.fr       */
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
		pthread_mutex_destroy(&d_list[i].mutex);
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
