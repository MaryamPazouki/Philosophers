/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 07:47:22 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-17 07:47:22 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_simulation(t_data *data, t_philo *philos)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	pthread_create(&monitor, NULL, monitor_philos, (void *)data);
	pthread_join(monitor, NULL);
	while (i < data->num_philos)
	{
		pthread_join(philos[i].thread, NULL);

		i++;
	}
}



int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (check_args(argc, argv))
		return (1);
	init_data(&data, argv);	
	philos = malloc(sizeof(t_philo) * data.num_philos);
	if (!philos)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}

	data.philos = philos;
	
	init_philosophers(&data, philos);
	start_simulation(&data, philos);

	free(philos);
	clean_up(&data);
	return (0);
}

//   ---------------------------original -----------------//
// #include "philo.h"

// void *monitor_philos(void *arg)
// {
// 	t_data *data = (t_data *)arg;
// 	int i;

// 	while (!data->dead)
// 	{
// 		i = 0;
// 		while (i < data->num_philos)
// 		{
// 			pthread_mutex_lock(&data->meal_check_lock);
// 			if (get_time() - data->philos[i].last_meal > data->time_to_die)
// 			{
// 				pthread_mutex_unlock(&data->meal_check_lock);
// 				pthread_mutex_lock(&data->write_lock);
// 				data->dead = 1;
// 				printf("%-6lld  %-2d  💀 died, lack of time\n", get_time()
//				- data->start_time, data->philos[i].id);
// 				pthread_mutex_unlock(&data->write_lock);
// 				return (NULL);
// 			}
// 			pthread_mutex_unlock(&data->meal_check_lock);
// 			i++;
// 			usleep(100);
// 		}
// 		if (data->must_eat != -1 && all_ate(data))
// 		{
// 			pthread_mutex_lock(&data->write_lock);
// 			data->dead = 1;
// 			printf("%-6lld  💀  All philosophers have eaten at least %d times\n",
// 				get_time() - data->start_time, data->must_eat);
// 			usleep(1000);
// 			pthread_mutex_unlock(&data->write_lock);
// 			return(NULL);
// 		}

// 		usleep(1000);
// 	}
// 	return (NULL);
// }

// void start_simulation(t_data *data, t_philo *philos)
// {
// 	pthread_t monitor;
// 	int i;
// 	i = 0;

// 	pthread_create(&monitor, NULL, monitor_philos, (void *)data);
// 	pthread_join(monitor, NULL);
// 	while(i < data->num_philos)
// 	{
// 		pthread_join(philos[i].thread, NULL);
// 		i++;
// 	}
// }

// void *philo_routine(void *arg)
// {
// 	t_philo *philo = (t_philo *)arg;

// 	if (philo->data->num_philos == 1)
// 	{
// 		pthread_mutex_lock(philo->left_fork);
// 		print_status(philo, "has taken a fork");
// 		smart_sleep(philo->data->time_to_die, philo->data);
// 		pthread_mutex_unlock(philo->left_fork);

// 		pthread_mutex_lock(&philo->data->write_lock);
// 		if (!philo->data->dead)
// 		{
// 			printf("%-6lld  %-2d  died,
//				waiting all the time for second fork!\n", get_time()
//				- philo->data->start_time, philo->id);
// 			philo->data->dead = 1;
// 		}
// 		pthread_mutex_unlock(&philo->data->write_lock);
// 		return (NULL);
// 	}

// 	if (philo->id % 2 != 0)
// 		usleep(1000);

// 	while (1)
// 	{

// 		if (philo->data->dead)
// 			break ;

// 		if (philo->id % 2 == 0)
// 		{
// 			pthread_mutex_lock(philo->right_fork);
// 			print_status(philo, "has taken a fork");
// 			pthread_mutex_lock(philo->left_fork);
// 			print_status(philo, "has taken a fork");
// 		}
// 		else
// 		{
// 			pthread_mutex_lock(philo->left_fork);
// 			print_status(philo, "has taken a fork");
// 			pthread_mutex_lock(philo->right_fork);
// 			print_status(philo, "has taken a fork");
// 		}

// 		pthread_mutex_lock(&philo->data->meal_check_lock);
// 		philo->last_meal = get_time();
// 		philo->meals_eaten += 1;
// 		pthread_mutex_unlock(&philo->data->meal_check_lock);

// 		print_status(philo, "is eating!");
// 		smart_sleep(philo->data->time_to_eat, philo->data);

// 		pthread_mutex_unlock(philo->left_fork);
// 		pthread_mutex_unlock(philo->right_fork);

// 		if (philo->data->dead)
// 			break ;

// 		print_status(philo, "is sleeping!");
// 		smart_sleep(philo->data->time_to_sleep, philo->data);

// 		if (philo->data->dead)
// 			break ;

// 		print_status(philo, "is thinking!");
// 		usleep(500);
// 	}
// 	usleep(1000);
// 	return (NULL);
// }

// int main(int argc, char **argv)
// {
// 	t_data data;
// 	t_philo *philos;

// 	if (check_args(argc, argv))
// 		return (1);
// 	init_data(&data, argv);
// 	philos = malloc(sizeof(t_philo) * data.num_philos);
// 	if (!philos)
// 	{
// 		printf("Error: Memory allocation failed.\n");
// 		return (1);
// 	}
// 	data.philos = philos;
// 	init_philosophers(&data, philos);
// 	start_simulation(&data, philos);
// 	free(philos);
// 	clean_up(&data);
// 	return (0);
// }