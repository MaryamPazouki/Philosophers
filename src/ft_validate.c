/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpazouki <mpazouki@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 09:16:13 by mpazouki          #+#    #+#             */
/*   Updated: 2025-04-17 09:16:13 by mpazouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			if (argv[i][j] < '0' || argv[i][j] > '9' || ft_atoi(argv[i]) <= 0 || ft_atoi(argv[i]) > 2147483647)
			{
				printf("Error: All arguments must be positive integers.\n");
				return (1);
			}
		}
	}
	return (0);
}