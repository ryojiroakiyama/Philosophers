/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_thread_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama  <rakiyama@student.42tokyo.j      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 21:31:14 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/25 21:47:04 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	copy_array(long original[], long copy[], int size)
{
	int	idx;

	idx = 0;
	while (idx < size)
	{
		copy[idx] = original[idx];
		idx++;
	}
}

static void	set_thread_data_philo(t_manage_data *mdata, t_thread_data *philo, \
																int philo_index)
{
	philo->order = philo_index + 1;
	philo->times_ate = 0;
	philo->times_must_eat = mdata->times_must_eat;
	copy_array(mdata->time, philo->time, TIME_NUM);
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->full = false;
	philo->is_full = &(philo->full);
	philo->is_died = &(mdata->die);
	philo->mutex[TO_RIGHT_FORK] = \
		mdata->mutexies + mdata->mutexinfo[FORKS][INDEX] + philo_index;
	philo->mutex[TO_LEFT_FORK] = \
		mdata->mutexies + mdata->mutexinfo[FORKS][INDEX] + \
								((philo_index + 1) % mdata->philo_num);
	philo->mutex[TO_LAST_EAT] = \
		mdata->mutexies + mdata->mutexinfo[LASTEATS][INDEX] + philo_index;
	philo->mutex[TO_PUT] = \
		mdata->mutexies + mdata->mutexinfo[PUTS][INDEX];
	philo->monitor = \
		mdata->threads + mdata->threinfo[MONITORS][INDEX] + philo_index;
}

static void	set_thread_data_monitor(t_thread_data *philo, \
									t_thread_data *monitor)
{
	monitor->order = philo->order;
	monitor->times_ate = philo->times_ate;
	monitor->times_must_eat = philo->times_must_eat;
	copy_array(philo->time, monitor->time, TIME_NUM);
	monitor->time_last_eat = philo->time_last_eat;
	monitor->full = philo->full;
	monitor->is_full = philo->is_full;
	monitor->is_died = philo->is_died;
	monitor->mutex[TO_RIGHT_FORK] = NULL;
	monitor->mutex[TO_LEFT_FORK] = NULL;
	monitor->mutex[TO_PUT] = philo->mutex[TO_PUT];
	monitor->mutex[TO_LAST_EAT] = philo->mutex[TO_LAST_EAT];
	monitor->monitor = NULL;
}

t_status	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;

	philo_index = mdata->philo_num;
	while (philo_index--)
	{
		a_philo = mdata->threads + mdata->threinfo[PHILOS][INDEX] + philo_index;
		set_thread_data_philo(mdata, a_philo, philo_index);
		set_thread_data_monitor(a_philo, a_philo->monitor);
	}
	return (SUCCESS);
}
