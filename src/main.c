#include "philo.h"


void print_status(t_philo *philo, char *msg)
{
    long long timestamp;

    // Lock the mutex to ensure only one thread prints at a time
    pthread_mutex_lock(&philo->data->write_lock);
    
    // Get the timestamp (current time in ms since the start of the simulation)
    timestamp = get_time() - philo->data->start_time;

    // Print philosopher status: timestamp, philosopher id, and message
    printf("%lld %d %s\n", timestamp, philo->id, msg);

    // Unlock the mutex so other threads can print
    pthread_mutex_unlock(&philo->data->write_lock);
}



void *monitor_philos(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;

    while (!data->dead)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->write_lock);
            if (get_time() - data->philos[i].last_meal > data->time_to_die)
            {
                print_status(&data->philos[i], "died");
                data->dead = 1;
                pthread_mutex_unlock(&data->write_lock);
                return (NULL);  // Stop checking, the simulation ends
            }
            pthread_mutex_unlock(&data->write_lock);
            i++;
        }
        usleep(1000); // Check every 1ms
    }
    return (NULL);
}




void start_simulation(t_data *data, t_philo *philos)
{
    pthread_t monitor;
    int i;

    i=0;
    pthread_create(&monitor, NULL, monitor_philos, (void *)data);
    pthread_detach(monitor);  // No need to join this thread
    while(i < data->num_philos)
    {
        pthread_join(philos[i].thread);
        i++;
    }
}





void *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg; // Correct type casting

    while (!philo -> data->dead) // Run until someone dies
    {
        // Take left fork
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork!");

        // Take right fork
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork!");

        // Eating
        philo->last_meal = get_time();
        philo->meals_eaten += 1;
        print_status(philo, "is eating!");
        usleep(philo->data->time_to_eat * 1000);

        // Put forks back
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        // Sleeping
        print_status(philo, "is sleeping!");
        usleep(philo->data->time_to_sleep * 1000);

        // Thinking
        print_status(philo, "is thinking!");
    }

    return (NULL);
}




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
}


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
    init_philosophers(&data, philos);
    start_simulation(&data, philos);
    free(philos);
    return (0);
}


https://www.youtube.com/watch?v=7ENFeb-J75k