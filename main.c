/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:27:49 by kaisogai          #+#    #+#             */
/*   Updated: 2025/10/12 18:36:07 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// スレッドで実行する関数
void	*worker(void *arg)
{
	struct timeval	tv;
	int				id;
	t_philo			*philo;

	philo = (t_philo *)arg;
	id = philo->id;
	for (int i = 0; i < 5; i++)
	{
		gettimeofday(&tv, NULL);
		tv.tv_usec /= 100;
		printf("%ld.%ld %d has taken a fork\n", tv.tv_sec, tv.tv_usec, id);
		sleep(1);
	}
	return (NULL);
}

// returns -1 when input has an invalid value
int	validate_input_nums(char **input, int input_count)
{
	int	i;
	int	is_error;

	i = 0;
	while (i < input_count)
	{
		is_error = 0;
		ft_atoi_error(*input, &is_error);
		if (is_error)
			return (-1);
		i++;
		input++;
	}
	return (0);
}

// number_of_philosophers
// time_to_die (in milliseconds)
// time_to_eat (in milliseconds)
// time_to_sleep (in milliseconds)
int	main(int argc, char **argv)
{
	t_philo	*philo;
	t_philo	*philo_first;
	int		i;

	i = 0;
	if (argc != 5)
		return (1);
	if (validate_input_nums(argv + 1, argc - 1) == -1)
		return (1);
	while (i < ft_atoi(argv[1]))
	{
		philo = malloc(sizeof(t_philo) * 1);
		philo_first = philo;
		philo->id = i;
		philo->fork_a = 0;
		philo->fork_b = 0;
		pthread_mutex_init(&philo->lock1, NULL);
		pthread_mutex_init(&philo->lock2, NULL);
		if (pthread_create(&philo->th, NULL, worker, &philo) != 0)
		{
			perror("pthread_create");
			return (1);
		}
		i++;
		fflush(stdout);
		philo = philo->next;
	}
	while (philo_first)
	{
		pthread_join(philo_first->th, NULL);
		philo_first = philo_first->next;
	}
	printf("all threads finished\n");
	return (0);
}
