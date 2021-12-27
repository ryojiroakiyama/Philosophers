/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 21:31:14 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/27 10:28:36 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_status	philo_eat(t_thread_data *philo)
{
	t_status	status;

	pthread_mutex_lock(philo->mutex[TO_RIGHT_FORK]);
	pthread_mutex_lock(philo->mutex[TO_LEFT_FORK]);
	if (philo->times_must_eat != UNSPECIFIED && \
		philo->times_ate >= philo->times_must_eat)
	{
		put_status(philo, RED, FULL, END_FULL);
		access_time_last_eat(philo, ZERO);
		status = FAIL;
	}
	else
	{
		put_status(philo, MAGENTA, FORK, CONTINUE);
		philo->times_ate++;
		status = put_status(philo, GREEN, EAT, CONTINUE);
		access_time_last_eat(philo, EDIT);
		if (status == SUCCESS && do_usleep(philo->time[TO_EAT] * 1000) == FAIL)
			status = FAIL;
	}
	pthread_mutex_unlock(philo->mutex[TO_RIGHT_FORK]);
	pthread_mutex_unlock(philo->mutex[TO_LEFT_FORK]);
	return (status);
}

static t_status	philo_sleep(t_thread_data *philo)
{
	t_status	status;

	status = put_status(philo, BLUE, SLEEP, CONTINUE);
	if (status == SUCCESS && do_usleep(philo->time[TO_SLEEP] * 1000) == FAIL)
		status = FAIL;
	return (status);
}

static t_status	philo_think(t_thread_data *philo)
{
	t_status	status;

	status = put_status(philo, CYAN, THINK, CONTINUE);
	return (status);
}

// (philo->mutex[TO_RIGHT_FORK] == philo->mutex[TO_LEFT_FORK])
// means there is only one philosopher.
void	*philo_action(void *data)
{
	t_thread_data	*philo;
	t_thread_data	*monitor;

	philo = (t_thread_data *)data;
	if (philo->mutex[TO_RIGHT_FORK] == philo->mutex[TO_LEFT_FORK])
	{
		put_status(philo, RED, DIE, END_DIE);
		return (data);
	}
	monitor = philo->monitor;
	if (thre_create(&(monitor->thread_id), &monitor_action, \
										monitor, "for monitor") || \
		(philo->order % 2 == 0 && do_usleep(PHILO_INTERVAL) == FAIL))
		put_status(philo, YEELOW, ERR, END_ERROR);
	while (1)
	{
		if (philo_eat(philo) == FAIL || philo_sleep(philo) == FAIL || \
			philo_think(philo) == FAIL)
			break ;
	}
	thre_join(monitor->thread_id, "for monitor");
	return (data);
}
