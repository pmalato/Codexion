/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 10:53:51 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/26 10:53:00 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_arguments
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}				t_arguments;

typedef struct s_coder
{
	int					id;
	int					compiled_times;
	struct timeval		deadline;
	int					last_compile;
	pthread_t			thread;
}				t_coder;

typedef struct s_dongle
{
	bool			state;
	int				cooldown;
	pthread_mutex_t	mutex;
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
int			is_nmb_of_args_valid(int ac);
int			ft_strcmp(char *s1, char *s2);
int			codex_atoi(char *str);
int			is_numeric_arg_valid(char *str);
int			is_fifo_or_edf(char *str);
size_t		c_list_len(t_coder *arr);
t_arguments	*args_parser(int ac, char **av);
t_dongle	new_dongle(void);
t_dongle	*dongle_list(t_arguments *parsed);
t_coder		new_coder(void);
t_coder		*coder_list(t_arguments *parsed, t_dongle *d_list);
t_thread	*new_thread_struct(\
	t_coder *coder, t_dongle *dongle, t_arguments *parsed);

#endif