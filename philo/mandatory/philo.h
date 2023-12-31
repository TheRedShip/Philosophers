/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:52:45 by ycontre           #+#    #+#             */
/*   Updated: 2024/01/05 19:57:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_glob
{
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				total_eaten;
	int				fork_num;
	u_int64_t		start_time;
	struct s_philo	*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	int				dead;
}	t_glob;

typedef struct s_philo
{
	int				id;
	int				status;
	int				eaten_time;
	u_int64_t		time_to_die;
	pthread_t		routine;
	pthread_t		supervisor;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	lock;
	struct s_glob	*glob;
}	t_philo;

void		ft_putstr(int fd, char *str);
int			ft_atoi(char *str);
void		error_exit(t_glob *glob);
void		clear_data(t_glob *glob);
void		print_message(t_philo *philo, char *message, int dead);

u_int64_t	get_time(void);
int			ft_usleep(useconds_t time);

t_glob		*init_glob(char **argv, int argc);

int			ft_handle_errors(int argc, char **argv);

void		*monitor(void *glob_pointer);
void		*supervisor(void *philo_pointer);
void		*routine(void *philo_pointer);

#endif