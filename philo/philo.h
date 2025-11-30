/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:28:07 by kaisogai          #+#    #+#             */
/*   Updated: 2025/10/12 18:39:54 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				fork_a;
	int				fork_b;
	pthread_mutex_t	lock1;
	pthread_mutex_t	lock2;
	pthread_t		th;
	struct s_philo	*next;
}					t_philo;

int	ft_atoi(const char *nptr);
int	ft_atoi_error(const char *nptr, int *is_error);

#endif
