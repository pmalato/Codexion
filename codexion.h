/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 10:53:51 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/30 20:19:13 by pmalato          ###   ########.fr       */
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

typedef struct s_coder
{
	bool				alive;
	bool				done;
	int					id;
	int					compiled_times;
	int					last_compile;
	long				deadline;
	pthread_t			thread;
	t_arguments			*parsed;
}				t_coder;

typedef struct s_dongle
{
	bool			state;
	long			cooldown;
	int				*queue;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}				t_dongle;

typedef struct s_thread
{
	t_coder		*coder;
	t_dongle	*d_list;
	t_arguments	*parsed;
}				t_thread;

void		pack_values(t_arguments *parsed, char **av);
void		*coder_routine(void *arg);
void		thread_cleanup(t_coder *c_list, size_t size);
void		cond_swap(int *a, int *b);
void		dongle_acquire(pthread_cond_t *cond, pthread_mutex_t *mutex,\
	t_dongle *d, t_thread *thread);
void		dongle_unlock(pthread_cond_t *c1, pthread_cond_t *c2,\
	pthread_mutex_t *m1, pthread_mutex_t *m2);
void		start_deadlines(t_arguments *parsed, t_coder *c_list);
void		free_coders(t_coder *c_list, int size);
int			is_nmb_of_args_valid(int ac);
int			ft_strcmp(char *s1, char *s2);
int			codex_atoi(char *str);
int			is_numeric_arg_valid(char *str);
int			is_fifo_or_edf(char *str);
int			thread_setup(t_coder *coder, size_t id,\
	t_dongle *d_list, t_arguments *parsed);
int			check_burnout(t_coder *c_list);
int			is_program_over(t_coder *c_list);
long		current_time(void);
long		compile(long c_time, t_thread *thread);
long		debug_and_refactor(long c_time, t_thread *thread);
long		dongle_handler(t_thread *thread);
t_arguments	*args_parser(int ac, char **av);
t_dongle	new_dongle(void);
t_dongle	*dongle_list(t_arguments *parsed);
t_coder		new_coder(void);
t_coder		*coder_list(t_arguments *parsed, t_dongle *d_list);
t_coder		*check_burned(t_coder *c_list);
t_thread	*new_thread_struct(\
	t_coder *coder, t_dongle *dongle, t_arguments *parsed);

#endif