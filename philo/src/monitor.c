/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 21:31:14 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/27 10:29:29 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_died(t_thread_data *monitor, long time_last_eat)
{
	long	time_now;
	long	time_diff;

	time_now = gettimeofday_mili();
	if (time_now == -1)
		return (true);
	time_diff = time_now - time_last_eat;
	if (time_diff >= monitor->time[TO_DIE])
	{
		put_status(monitor, RED, DIE, END_DIE);
		return (true);
	}
	if (do_usleep(MONITOR_INTERVAL) == FAIL)
		return (true);
	return (false);
}

void	*monitor_action(void *data)
{
	t_thread_data	*monitor;
	long			time_last_eat;

	monitor = (t_thread_data *)data;
	while (1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		if (time_last_eat == -1)
			break ;
		if (is_died(monitor, time_last_eat))
			break ;
	}
	put_status(monitor, YEELOW, ERR, END_ERROR);
	return (data);
}
