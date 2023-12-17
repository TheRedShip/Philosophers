/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 13:53:05 by ycontre           #+#    #+#             */
/*   Updated: 2023/12/17 15:03:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

u_int64_t	get_time(t_glob *glob)
{
	struct timeval	tv;
	
	if (gettimeofday(&tv, NULL))
		error_exit(glob);
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

void print_message(t_philo *philo, char *message)
{
	pthread_mutex_lock(&(philo->glob->lock));
	if (philo->glob->dead == 0)
		printf("%ld %d %s\n", get_time(philo->glob) - philo->glob->start_time, philo->id, message);
	pthread_mutex_unlock(&(philo->glob->lock));
}

int	ft_usleep(useconds_t time, t_glob *glob)
{
	u_int64_t	start;
	start = get_time(glob);
	while ((get_time(glob) - start) < time)
		usleep(time / 10);
	return(0);
}

void do_eat(t_philo *philo)
{
	philo->status = 0;
	pthread_mutex_lock(philo->right_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->lock);
	philo->status = 1;
	philo->time_to_die = get_time(philo->glob) + philo->glob->time_to_die;
	print_message(philo, "is eating");
	philo->eaten_time = get_time(philo->glob);
	ft_usleep(philo->glob->time_to_eat, philo->glob);
	philo->status = 2;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
void	*supervisor(void *philo_pointer)
{
	t_philo *philo;

	philo = (t_philo *)philo_pointer;
	while (philo->glob->dead == 0)
	{
		pthread_mutex_lock(&(philo->lock));
		if (get_time(philo->glob) >= philo->time_to_die && philo->status != 1)
		{
			pthread_mutex_lock(&(philo->glob->lock));
			if (philo->glob->dead == 0)
				printf("%ld %d %s\n", get_time(philo->glob) - philo->glob->start_time, philo->id, "died");
			philo->glob->dead = 1;
			pthread_mutex_unlock(&(philo->glob->lock));
		}
		pthread_mutex_unlock(&(philo->lock));
	}
	return ((void *)0);
}

void *routine(void *philo_pointer)
{
	t_philo *philo;

	philo = (t_philo *)philo_pointer;
	philo->time_to_die = philo->glob->time_to_die + get_time(philo->glob);
	if (pthread_create(&(philo->supervisor), NULL, &supervisor, (void *)philo))
		error_exit(philo->glob);
	while (philo->glob->dead == 0)
	{
		do_eat(philo);
		philo->status = 3;
		print_message(philo, "is sleeping");
		ft_usleep(philo->glob->time_to_sleep, philo->glob);
		philo->status = 4;
		print_message(philo, "is thinking");
		ft_usleep(2, philo->glob);
	}
	if (pthread_join(philo->supervisor, NULL))
		error_exit(philo->glob);
	return ((void *)0);
}

void calcul_philo(t_glob *glob)
{
	int	i;
	
	glob->start_time = get_time(glob);
	i = 0;
	while (i < glob->philo_num)
	{
		if (pthread_create(&(glob->philo[i].routine), NULL, &routine, &glob->philo[i]))
			error_exit(glob);
		i++;
	}
	i = 0;
	while (i < glob->philo_num)
	{
		if(pthread_join(glob->philo[i].routine, NULL))
			error_exit(glob);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_glob *glob;

	if (ft_handle_errors(argc, argv) == 0)
		return (0);
	glob = init_glob(argv, argc);
	calcul_philo(glob);
	clear_data(glob);
	return (0);
}