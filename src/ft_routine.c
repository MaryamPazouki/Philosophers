/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-22 10:04:43 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-22 10:04:43 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		smart_sleep(philo->data->time_to_die, philo->data);
		pthread_mutex_unlock(philo->left_fork);

		pthread_mutex_lock(&philo->data->write_lock);
		if (!philo->data->dead)
		{
			printf("%-6lld  %-2d  died, waiting all the time for second fork!\n", get_time() - philo->data->start_time, philo->id);
			philo->data->dead = 1;
		}
		pthread_mutex_unlock(&philo->data->write_lock);
		return NULL;
	}

	if (philo->id % 2 != 0)
		usleep(1000);

	while (1)
	{

		if (philo->data->dead)
			break;

		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}

		pthread_mutex_lock(&philo->data->meal_check_lock);
		philo->last_meal = get_time();
		philo->meals_eaten += 1;
		pthread_mutex_unlock(&philo->data->meal_check_lock);

		print_status(philo, "is eating!");
		smart_sleep(philo->data->time_to_eat, philo->data);

		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		if (philo->data->dead)
			break;

		print_status(philo, "is sleeping!");
		smart_sleep(philo->data->time_to_sleep, philo->data);

		if (philo->data->dead)
			break;

		print_status(philo, "is thinking!");
		usleep(500);
	}
	usleep(1000);
	return NULL;
}