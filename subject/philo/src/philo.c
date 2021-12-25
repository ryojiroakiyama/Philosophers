#include "philo.h"

static t_status	philo_eat(t_thread_data *philo)
{
	t_status	status;

	pthread_mutex_lock(philo->mutex[TO_RIGHT_FORK]);
	pthread_mutex_lock(philo->mutex[TO_LEFT_FORK]);
	access_time_last_eat(philo, EDIT);
	status = put_status(philo, GREEN, EAT, CONTINUE);
	if (status == SUCCESS && do_usleep(philo->time[TO_EAT] * 1000) == FAIL)
		status = FAIL;
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

static t_status	special_cases(t_thread_data *philo)
{
	if (philo->mutex[TO_RIGHT_FORK] == philo->mutex[TO_LEFT_FORK])
	{
		put_status(philo, RED, DIE, END_DIE);
		return (SUCCESS);
	}
	else if (philo->times_must_eat == 0)
	{
		put_status(philo, MAGENTA, FULL, END_FULL);
		return (SUCCESS);
	}
	return (FAIL);
}

void	*philo_action(void *data)
{
	t_thread_data	*philo;
	t_thread_data	*monitor;

	philo = (t_thread_data *)data;
	if (special_cases(philo) == SUCCESS)
		return (data);
	monitor = philo->monitor;
	if (thre_create(&(monitor->thread_id), &monitor_action, \
										monitor, "for monitor") || \
		(philo->order % 2 == 0 && do_usleep(INTERVAL) == FAIL))
	{
		put_status(philo, YEELOW, ERR, END_ERROR);
		return (data);
	}
	while (1)
	{
		if (philo_eat(philo) == FAIL || philo_sleep(philo) == FAIL || \
			philo_think(philo) == FAIL)
			break ;
	}
	thre_join(monitor->thread_id, "for monitor");
	return (data);
}
