/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 11:15:55 by pmalato           #+#    #+#             */
/*   Updated: 2026/08/01 17:19:30 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_arguments
{
	long			clock_start;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	pthread_mutex_t	main_lock;
}				t_arguments;

typedef struct s_queue
{
	int	queue[2];
	int	size;
}				t_queue;

typedef struct s_coder
{
	bool				alive;
	bool				done;
	int					id;
	int					compiled_times;
	int					last_compile;
	long				deadline;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	t_arguments			*parsed;
}				t_coder;

typedef struct s_dongle
{
	bool			state;
	long			cooldown;
	t_queue			*queue;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_dongle;

typedef struct s_thread
{
	t_coder		*coder;
	t_dongle	*d_list;
	t_arguments	*parsed;
}				t_thread;

typedef struct s_monitor
{
	t_coder		*coders;
	t_dongle	*dongles;
	t_arguments	*parsed;
}				t_monitor;

#endif