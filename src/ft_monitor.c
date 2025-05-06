/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-22 10:03:24 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-22 10:03:24 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_lack_of_time(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_check_lock);
		if (get_time() - data->philos[i].last_meal > data->time_to_die)
		{
			pthread_mutex_unlock(&data->meal_check_lock);
			pthread_mutex_lock(&data->write_lock);
			data->dead = 1;
			printf("%-6lld  %-2d  💀 died, lack of time\n", get_time()
				- data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->write_lock);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_check_lock);
		i++;
		usleep(100);
	}
	return (0);
}

void	*monitor_philos(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!data->dead)
	{
		if (ft_lack_of_time(data))
			return (NULL);
		if (data->must_eat != -1 && all_ate(data))
		{
			pthread_mutex_lock(&data->write_lock);
			data->dead = 1;
			printf("%-6lld  💀  All philosophers have eaten at least %d times\n",
				get_time() - data->start_time, data->must_eat);
			usleep(1000);
			pthread_mutex_unlock(&data->write_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

// int		g_flag = 0;

// void	reaction(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		printf("\n Terminating simulation...\n killed by SIGINT\n");
// 		g_flag = 1;
// 	}
// }
// int	ft_lack_of_time(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->num_philos)
// 	{
// 		pthread_mutex_lock(&data->meal_check_lock);
// 		if (get_time() - data->philos[i].last_meal > data->time_to_die
//			|| g_flag == 1)
// 		{
// 			pthread_mutex_unlock(&data->meal_check_lock);
// 			pthread_mutex_lock(&data->write_lock);
// 			data->dead = 1;
// 			printf("%-6lld  %-2d  💀 died, lack of time\n", get_time()
// 				- data->start_time, data->philos[i].id);
// 			pthread_mutex_unlock(&data->write_lock);
// 			return (1);
// 		}
// 		pthread_mutex_unlock(&data->meal_check_lock);
// 		i++;
// 		usleep(100);
// 	}
// 	return (0);
// }
// void	*monitor_philos(void *arg)
// {
// 	t_data	*data;

// 	data = (t_data *)arg;
// 	signal(SIGINT, reaction);
// 	while (!data->dead)
// 	{
// 		if (ft_lack_of_time(data))
// 			return (NULL);
// 		if (data->must_eat != -1 && all_ate(data))
// 		{
// 			pthread_mutex_lock(&data->write_lock);
// 			data->dead = 1;
// 			printf("%-6lld  💀  All philosophers have eaten at least %d times\n",
// 				get_time() - data->start_time, data->must_eat);
// 			usleep(1000);
// 			pthread_mutex_unlock(&data->write_lock);
// 			return (NULL);
// 		}
// 		usleep(1000);
// 	}
// 	return (NULL);
// }