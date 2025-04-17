/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 09:09:09 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-17 09:09:09 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

int	ft_atoi(const char *str)
{
	int		i = 0;
	int		sign = 1;
	long	result = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (sign * result);
}

/*Purpose: Thread-safe function to print philosopher actions (e.g., "is eating", "died").
Locks the write_lock mutex to prevent jumbled output from concurrent threads.
Gets the current time minus the simulation start time (get_time() - start_time).
Prints the message with timestamp and philosopher ID.
Unlocks the mutex.
➡️ Ensures clean and synchronized terminal output.*/
	
void	print_status(t_philo *philo, char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	if (!philo->data->dead || strcmp(msg, "died") == 0)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%-6lld  %-2d  %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
}



int all_ate(t_data *data)
{
	int i;
	int full_philos;

	i = 0;
	full_philos = 0;
	pthread_mutex_lock(&data->meal_check_lock);
	while(i < data->num_philos)
	{
		if (data->must_eat != -1 && data->philos[i].meals_eaten >= data-> must_eat)
			full_philos++;
		i++;
	}
	pthread_mutex_unlock(&data->meal_check_lock);
	return(full_philos == data->num_philos);
}


//Use this instead of usleep() for accurate timing:

void smart_sleep(int duration_ms, t_data *data)
{
	long long start = get_time();

	while (!data->dead && (get_time() - start < duration_ms))
	{	usleep(100); // sleep in small chunks, check `dead` flag
		//printf("smart_sleep running: %lld ms\n", get_time() - start);
	}
}


void clean_up(t_data *data)
{
    // Free the dynamically allocated forks
    if (data->forks) {
        free(data->forks);  // Free the forks array
    }
    
    // Clean up any mutexes if needed (usually doesn't require freeing)
    pthread_mutex_destroy(&data->write_lock);
    pthread_mutex_destroy(&data->meal_check_lock);

    // Add any other cleanup code here as necessary
}