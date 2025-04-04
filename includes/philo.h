
#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_philo {
    int id;
    int meals_eaten;
    long long last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_data *data;
    pthread_t thread;
} t_philo;

typedef struct s_data {
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int must_eat;
    long long start_time;
    int dead;
    pthread_mutex_t *forks;
    pthread_mutex_t write_lock;
} t_data;

// Function prototypes
long long get_time(void);
void init_data(t_data *data, char **argv);
void init_philosophers(t_data *data, t_philo *philos);
void start_simulation(t_data *data, t_philo *philos);
void *philo_routine(void *arg);
void print_status(t_philo *philo, char *msg);

#endif
