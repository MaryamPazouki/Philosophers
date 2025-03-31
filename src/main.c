#include "philo.h"


void init_data(t_data *data, char **argv)
{
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
    
    for (i = 0; i < data->num_philos; i++)
        pthread_mutex_init(&data->forks[i], NULL);

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
