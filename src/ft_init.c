/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 09:13:58 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-17 09:13:58 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv)
{
	int	i;

	i = -1;
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->dead = 0;
	data->start_time = get_time();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		printf("Error: Memory allocation failed.\n");
		exit(1);
	}
	while (++i < data->num_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->meal_check_lock, NULL);
}

void	init_philosophers(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].data = data;
		if (pthread_create(&philos[i].thread, NULL,
				philo_routine, &philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d.\n", 
				philos[i].id);
			exit(1);
		}
		i++;
	}
}

// void	init_data(t_data *data, char **argv)
// {
// 	int	i;
// 	i=0;
// 	data->num_philos = ft_atoi(argv[1]);
// 	data->time_to_die = ft_atoi(argv[2]);
// 	data->time_to_eat = ft_atoi(argv[3]);
// 	data->time_to_sleep = ft_atoi(argv[4]);
// 	data->must_eat = (argv[5]) ? ft_atoi(argv[5]): -1;
// 	data->dead = 0;
// 	data->start_time = get_time();

// 	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
// 	if (!data->forks)
// 	{
// 		printf("Error: Memory allocation failed.\n");
// 		exit(1);
// 	}
// 	while(i< data->num_philos )
// 	{
// 		pthread_mutex_init(&data->forks[i], NULL);
// 		i++;
// 	}
// 	pthread_mutex_init(&data->write_lock, NULL);
// 	pthread_mutex_init(&data->meal_check_lock, NULL);
// }