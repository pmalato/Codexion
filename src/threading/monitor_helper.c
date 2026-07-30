/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 18:24:29 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/30 20:01:26 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

void	free_coders(t_coder *c_list, int size)
{
	int	i;

	i = 0;
	while (i < size)
		free(&c_list[i]);
}

void	start_deadlines(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		c_list[i].deadline = c_list->parsed->clock_start + current_time();
		i++;
	}
}

int	check_burnout(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		if (c_list[i].deadline <= current_time())
		{
			c_list[i].alive = false;
			return (0);
		}
		i++;
	}
}

t_coder	*check_burned(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		if (!c_list[i].alive)
			return (&c_list[i]);
		i++;
	}
	return (NULL);
}

int	is_program_over(t_coder *c_list)
{
	int	i;

	i = 0;
	while (i < c_list->parsed->number_of_coders)
	{
		if (!c_list[i].done)
			return (0);
		i++;
	}
	return (1);
}
