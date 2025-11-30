/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:26:45 by kaisogai          #+#    #+#             */
/*   Updated: 2025/10/12 18:35:32 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_space(char c)
{
	if (c == ' ' || c == '\f' || c == '\r' || c == '\n' || c == '\t'
		|| c == '\v')
		return (1);
	return (0);
}

static int	roop(int *i, const char *nptr, int is_minus, int *result)
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
