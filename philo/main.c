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

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long ms)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(100);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id + 1, status);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	return (0);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	if (philo->data->philo_num == 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		ft_usleep(philo->data->time_to_die);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*worker(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		ft_usleep(1);
	while (!check_death(philo))
	{
		philo_eat(philo);
		if (check_death(philo))
			break ;
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
			break ;
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		if (check_death(philo))
			break ;
		print_status(philo, "is thinking");
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

void	*monitor(void *arg)
{
	t_philo	*philos;
	int		i;
	long	time_since_meal;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philos->data->philo_num)
		{
			time_since_meal = get_time_ms() - philos[i].last_meal_time;
			if (time_since_meal > philos->data->time_to_die)
			{
				pthread_mutex_lock(&philos->data->death_lock);
				philos->data->someone_died = 1;
				pthread_mutex_unlock(&philos->data->death_lock);
				pthread_mutex_lock(&philos->data->print_lock);
				printf("%ld %d died\n", get_time_ms() - philos->data->start_time,
					philos[i].id + 1);
				pthread_mutex_unlock(&philos->data->print_lock);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	initialize(t_philo **philos, pthread_mutex_t **forks, t_data *data)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->philo_num);
	*forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_init(&(*forks)[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->philo_num)
	{
		(*philos)[i].id = i;
		(*philos)[i].left_fork = &(*forks)[i];
		(*philos)[i].right_fork = &(*forks)[(i + 1) % data->philo_num];
		(*philos)[i].last_meal_time = get_time_ms();
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].data = data;
		i++;
	}
}

void	cleanup(t_philo *philos, pthread_mutex_t *forks, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death_lock);
	free(philos);
	free(forks);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	if (validate_input_nums(argv + 1, argc - 1) == -1)
		return (1);
	data->philo_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	data->someone_died = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		monitor_th;
	t_data			data;
	int				i;

	if (init_data(&data, argc, argv) != 0)
		return (1);
	initialize(&philos, &forks, &data);
	data.start_time = get_time_ms();
	i = 0;
	while (i < data.philo_num)
	{
		philos[i].last_meal_time = data.start_time;
		if (pthread_create(&philos[i].th, NULL, worker, &philos[i]) != 0)
		{
			perror("pthread_create");
			return (1);
		}
		i++;
	}
	pthread_create(&monitor_th, NULL, monitor, philos);
	i = 0;
	while (i < data.philo_num)
	{
		pthread_join(philos[i].th, NULL);
		i++;
	}
	pthread_join(monitor_th, NULL);
	cleanup(philos, forks, &data);
	return (0);
}
