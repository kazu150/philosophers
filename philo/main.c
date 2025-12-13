/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:27:49 by kaisogai          #+#    #+#             */
/*   Updated: 2025/12/13 22:45:13 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// スレッドで実行する関数
void	*worker(void *arg)
{
	struct timeval	tv;
	t_philo			*philo;
	int				id;

	philo = (t_philo *)arg;
	id = philo->id;
	pthread_mutex_lock(&philo->lock);
	pthread_mutex_lock(&philo->next->lock);
	gettimeofday(&tv, NULL);
	tv.tv_usec /= 100;
	printf("%ld.%ld %d has taken a fork\n", (long)tv.tv_sec, (long)tv.tv_usec,
		id);
	printf("%ld.%ld %d is eating\n", (long)tv.tv_sec, (long)tv.tv_usec, id);
	sleep(3);
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->next->lock);
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

void	initialize(t_philo **philo, int philo_num)
{
	int		i;
	t_philo	*philo_first;
	t_philo	*prev;

	philo_first = NULL;
	prev = NULL;
	i = 0;
	while (i < philo_num)
	{
		*philo = malloc(sizeof(t_philo) * 1);
		(*philo)->id = i;
		(*philo)->next = NULL;
		pthread_mutex_init(&(*philo)->lock, NULL);
		if (!philo_first)
			philo_first = *philo;
		if (prev)
			prev->next = *philo;
		prev = *philo;
		i++;
	}
	prev->next = philo_first;
	*philo = philo_first;
}

// number_of_philosophers
// time_to_die (in milliseconds)
// time_to_eat (in milliseconds)
// time_to_sleep (in milliseconds)
int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		philo_num;
	int		i;

	philo = NULL;
	i = 0;
	if (argc != 5)
		return (1);
	if (validate_input_nums(argv + 1, argc - 1) == -1)
		return (1);
	philo_num = ft_atoi(argv[1]);
	initialize(&philo, philo_num);
	while (i < philo_num)
	{
		philo->id = i;
		if (pthread_create(&philo->th, NULL, worker, philo) != 0)
		{
			perror("pthread_create");
			return (1);
		}
		i++;
		philo = philo->next;
	}
	i = 0;
	while (i < philo_num)
	{
		pthread_join(philo->th, NULL);
		philo = philo->next;
		i++;
	}
	printf("all threads finished\n");
	return (0);
}
