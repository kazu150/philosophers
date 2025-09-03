#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:27:49 by kaisogai          #+#    #+#             */
/*   Updated: 2025/06/09 16:09:11 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_philo
{
	int				id;
	int				fork_a;
	int				fork_b;
	pthread_mutex_t	lock1;
	pthread_mutex_t	lock2;
}					t_philo;

static int	is_space(char c)
{
	if (c == ' ' || c == '\f' || c == '\r' || c == '\n' || c == '\t'
		|| c == '\v')
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	is_minus;
	int	result;

	is_minus = 0;
	i = 0;
	result = 0;
	while (is_space(nptr[i]))
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		is_minus = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (is_minus)
			result = (result * 10 - (nptr[i] - '0'));
		else
			result = (result * 10 + (nptr[i] - '0'));
		i++;
	}
	return (result);
}

int	roop(int *i, const char *nptr, int is_minus, int *result)
{
	int	prev_result;
	int	is_no_digits_error;
	int	is_overflow_error;

	is_overflow_error = 0;
	is_no_digits_error = 1;
	while (nptr[*i] >= '0' && nptr[*i] <= '9')
	{
		is_no_digits_error = 0;
		prev_result = *result;
		if (is_minus)
		{
			*result = (*result * 10 - (nptr[*i] - '0'));
			if (prev_result < *result)
				is_overflow_error = 1;
		}
		else
		{
			*result = (*result * 10 + (nptr[*i] - '0'));
			if (prev_result > *result)
				is_overflow_error = 1;
		}
		(*i)++;
	}
	return (is_no_digits_error || is_overflow_error);
}

// ft_atoi with "is_error flag" version
// error contains non-integer values, values exceed integer
int	ft_atoi_error(const char *nptr, int *is_error)
{
	int	i;
	int	is_minus;
	int	result;

	is_minus = 0;
	i = 0;
	result = 0;
	while (is_space(nptr[i]))
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		is_minus = 1;
		i++;
	}
	*is_error = roop(&i, nptr, is_minus, &result);
	if (((nptr[i] < '0' || nptr[i] > '9') && nptr[i] != 0))
		*is_error = 1;
	return (result);
}

// スレッドで実行する関数
void	*worker(void *arg)
{
	struct timeval	tv;
	int				id;

	t_philo *philo;         // 構造体へのポインタを用意
	philo = (t_philo *)arg; // void* → t_philo* にキャスト
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
	int				fork1;
	int				fork2;
	pthread_mutex_t	lock1;
	pthread_mutex_t	lock2;
	t_philo			philo1;
	t_philo			philo2;

	fork1 = 0;
	fork2 = 0;
	int id1, id2;
	id1 = 1, id2 = 2;
	pthread_t th1, th2;
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);
	philo1.id = id1;
	philo1.fork_a = fork1;
	philo1.fork_b = fork2;
	philo1.lock1 = lock1;
	philo1.lock2 = lock2;
	philo2.id = id1;
	philo2.fork_a = fork1;
	philo2.fork_b = fork2;
	philo2.lock1 = lock1;
	philo2.lock2 = lock2;
	if (argc != 5)
		return (1);
	if (validate_input_nums(argv + 1, argc - 1) == -1)
		return (1);
	if (pthread_create(&th1, NULL, worker, &philo1) != 0)
	{
		perror("pthread_create");
		return (1);
	}
	if (pthread_create(&th2, NULL, worker, &philo2) != 0)
	{
		perror("pthread_create");
		return (1);
	}
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	printf("both threads finished\n");
	return (0);
}
