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





// void	print_status(t_philo *philo, char *msg)
// {
// 	long long	timestamp;

// 	pthread_mutex_lock(&philo->data->write_lock);
// 	if (!philo->data->dead)
// 	{
// 		timestamp = get_time() - philo->data->start_time;
// 		printf("%-6lld  %-2d  %s\n", timestamp, philo->id, msg);
// 	}
// 	else if (philo->data->dead)
// 	{
// 		printf("%-2d  %s\n", philo->id, msg);
// 		pthread_mutex_unlock(&philo->data->write_lock);
// 	}
// 	pthread_mutex_unlock(&philo->data->write_lock);
// }


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



/*Purpose: Runs in a separate thread to monitor if any philosopher dies.

Casts the arg back to t_data *.
Loops through all philosophers checking if:
current_time - last_meal > time_to_die.
If so, prints "died" and sets the dead flag.
Loops every 1ms to avoid CPU overuse.
➡️ Stops simulation once someone dies.*/

void *monitor_philos(void *arg)
{
	t_data *data = (t_data *)arg;
	int i;
	
	while (!data->dead)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->meal_check_lock);
			long long time_since_meal = get_time() - data->philos[i].last_meal;
			
			// check if philo has died
			if (time_since_meal > data->time_to_die)
			{
				pthread_mutex_unlock(&data->meal_check_lock);
				pthread_mutex_lock(&data->write_lock);
				data->dead = 1;
				printf("%-6lld  %-2d  died, lack of time \n", get_time() - data->start_time, data->philos[i].id);
				return (NULL);
			}
			pthread_mutex_unlock(&data->meal_check_lock);
			i++;
			usleep(100); // reduce CPU usage
		}
		// check if the philos has eaten enough
		if (data->must_eat != -1 && all_ate(data))
		{
			pthread_mutex_lock(&data->write_lock);
			data->dead = 1;
			printf("%-6lld  💀  All philosophers have eaten at least %d times\n",
				get_time() - data->start_time, data->must_eat);
			//usleep(1000);
			pthread_mutex_unlock(&data->write_lock);
			return(NULL);
		}

		usleep(1000); // Check every 1ms
	}
	return (NULL);
}

/*Purpose: Launches the monitor and joins philosopher threads.
Creates and detaches the monitoring thread.
Waits (pthread_join) for each philosopher thread to finish.
➡️ Ties the whole simulation together.*/


void start_simulation(t_data *data, t_philo *philos)
{
    pthread_t monitor;
    int i;
	i = 0;

    // Create the monitor thread
    pthread_create(&monitor, NULL, monitor_philos, (void *)data);

    // Wait for the monitor thread to finish
    pthread_join(monitor, NULL);
	while(i < data->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
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



/*Purpose: The core behavior loop for each philosopher.
Loops while no one is dead:
Takes left fork (pthread_mutex_lock)
Takes right fork
Updates last_meal, eats, and increases meal count.
Unlocks both forks.
Sleeps
Thinks
➡️ Each thread runs this — it’s the philosopher’s lifecycle.*/

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;


	// Handle case where there's only 1 philosopher
    if (philo->data->num_philos == 1)
	{
    	pthread_mutex_lock(philo->left_fork);
    	print_status(philo, "has taken a fork");
    	// He can't take a second fork, so he waits until he dies
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


	// Small delay to reduce deadlock chances
	if (philo->id % 2 != 0)
		usleep(1000);

	while (1)
	{
		// Check if simulation is over
		if (philo->data->dead)
			break;

		// Pick up forks (prevent deadlock via fork order)
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

		// Eating
		pthread_mutex_lock(&philo->data->meal_check_lock);
		philo->last_meal = get_time();
		philo->meals_eaten += 1;
		pthread_mutex_unlock(&philo->data->meal_check_lock);

		print_status(philo, "is eating!");
		smart_sleep(philo->data->time_to_eat, philo->data);
				
		// Put down forks
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		if (philo->data->dead)
			break;

		// Sleeping
		print_status(philo, "is sleeping!");
		smart_sleep(philo->data->time_to_sleep, philo->data);

		if (philo->data->dead)
			break;

		// Thinking
		print_status(philo, "is thinking!");
		usleep(500); // small wait to reduce contention
	}
	usleep(1000);
	return NULL;
}



/*
Purpose: Parses arguments and sets up shared data.
Converts all arguments to integers.
Initializes dead, start_time.
Allocates and initializes all mutexes for forks.
Initializes the write_lock.

Prepares global/shared data used by all philosophers.*/

void init_data(t_data *data, char **argv)
{
	int i;

	i=0;
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = (argv[5]) ? ft_atoi(argv[5]): -1;
	
	data->dead = 0;
	data->start_time = get_time();
	
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		printf("Error: Memory allocation failed.\n");
		exit(1);
	}
	
	while(i< data->num_philos )
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}     

	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->meal_check_lock, NULL);
}


/*Initializes each philosopher’s data and spawns their threads.

Sets:
id, meals_eaten, last_meal
left_fork and right_fork
data reference

Creates threads that run philo_routine.
Makes each philosopher a thread with proper fork pointers.*/


void init_philosophers(t_data *data, t_philo *philos)
{
	int i;

	for (i = 0; i < data->num_philos; i++)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].data = data;

		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d.\n", philos[i].id);
			exit(1);
		}
	}
}



/*
Purpose: Validates the command-line arguments.
Must be 5 or 6 arguments.
All arguments must be positive integers.

➡️ Catches early errors before simulating.*/


int check_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	for (int i = 1; i < argc; i++)
	{
		for (int j = 0; argv[i][j]; j++)
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: All arguments must be positive integers.\n");
				return (1);
			}
		}
	}
	return (0);
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


int main(int argc, char **argv)
{
	t_data data;
	t_philo *philos;
 
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
	clean_up(&data);  // Clean up allocated memory
	return (0);
}


// https://www.youtube.com/watch?v=7ENFeb-J75k