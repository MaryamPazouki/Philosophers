/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 07:47:33 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-17 07:47:33 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
#include <signal.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
	pthread_t		thread;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long long		start_time;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_check_lock;
	struct s_philo	*philos;
}					t_data;

// Function prototypes
long long			get_time(void);
void				init_data(t_data *data, char **argv);
void				init_philosophers(t_data *data, t_philo *philos);
void				start_simulation(t_data *data, t_philo *philos);
void				*philo_routine(void *arg);
void				print_status(t_philo *philo, char *msg);
int					ft_atoi(const char *str);
void				print_status(t_philo *philo, char *msg);
int					all_ate(t_data *data);
void				smart_sleep(int duration_ms, t_data *data);
void				init_philosophers(t_data *data, t_philo *philos);
void				init_data(t_data *data, char **argv);
int					check_args(int argc, char **argv);
void				clean_up(t_data *data);
void				*monitor_philos(void *arg);
#endif
