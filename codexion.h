/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecoelho <pecoelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 10:53:51 by pmalato           #+#    #+#             */
/*   Updated: 2026/09/07 19:02:15 by pecoelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_arguments
{
	bool			stop;
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
	pthread_mutex_t	print;
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
	t_coder		*c_list;
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

void		safe_print(pthread_mutex_t *lock, char *str, long time, int id);
void		pack_values(t_arguments *parsed, char **av);
void		*coder_routine(void *arg);
void		thread_cleanup(t_coder *c_list, size_t size);
void		dongle_mutex_cleanup(t_dongle *d_list, size_t size);
void		coder_mutex_cleanup(t_coder *c_list, size_t size);
void		cond_swap(int *a, int *b);
void		start_deadlines(t_coder *c_list);
void		init_coder_mutex(t_coder *c_list);
void		free_coders(t_coder *c_list, int size);
void		monitor_cleanup(t_arguments *parsed, t_coder *c_list,\
	t_dongle *d_list, size_t i);
void		dongle_release(t_thread *thread, size_t id);
void		dongle_broadcast(t_dongle *d_list, t_coder *c_list);
void		end_monitor(t_monitor *monitor, pthread_t monitor_thread);
void		request_stop(t_arguments *parsed);
void		edf(t_thread *thread, t_dongle *dongle);
bool		check_stop(t_arguments *parsed);
bool		get_coder_alive(t_coder *coder);
int			dongle_acquire(t_dongle *d, t_thread *thread);
int			monitor_thread(t_arguments *parsed, t_dongle *d_list,\
	t_coder *c_list);
int			is_nmb_of_args_valid(int ac);
int			ft_strcmp(char *s1, char *s2);
int			codex_atoi(char *str);
int			is_numeric_arg_valid(char *str);
int			is_fifo_or_edf(char *str);
int			thread_setup(t_coder *coder, size_t id, t_dongle *d_list);
int			check_burnout(t_dongle *d_list, t_coder *c_list);
int			is_program_over(t_coder *c_list);
long		current_time(void);
void		compile(long c_time, t_thread *thread);
void		debug_and_refactor(long c_time, t_thread *thread);
long		dongle_handler(t_thread *thread);
long		get_coder_deadline(t_coder *coder);
t_arguments	*args_parser(int ac, char **av);
t_dongle	new_dongle(void);
t_dongle	*dongle_list(t_arguments *parsed);
t_coder		new_coder(void);
t_coder		*coder_list(t_arguments *parsed);
t_thread	*new_thread_struct(t_coder *c_list, t_coder *coder,\
	t_dongle *dongle, t_arguments *parsed);
t_monitor	*new_monitor_struct(t_arguments *parsed,\
	t_dongle *d_list, t_coder *c_list);

#endif