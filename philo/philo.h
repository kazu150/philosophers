/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:28:07 by kaisogai          #+#    #+#             */
/*   Updated: 2025/12/13 22:16:27 by kaisogai         ###   ########.fr       */
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
	pthread_mutex_t	lock;
	pthread_t		th;
	struct s_philo	*next;
}					t_philo;

int	ft_atoi(const char *nptr);
int	ft_atoi_error(const char *nptr, int *is_error);

#endif
