#include "philo.h"

t_life	access_life_flag(t_thread_data *thread, t_access mode)
{
	t_life	result;

	pthread_mutex_lock(thread->mutex[TO_LIFE_FLAG]);
	if (mode == EDIT)
		*(thread->life_flag) = SOME_ONE_DIED;
	result = *(thread->life_flag);
	pthread_mutex_unlock(thread->mutex[TO_LIFE_FLAG]);
	return (result);
}

long	access_time_last_eat(t_thread_data *thread, t_access mode)
{
	long	result;

	pthread_mutex_lock(thread->mutex[TO_LAST_EAT]);
	if (mode == EDIT)
 		*(thread->time_last_eat) = gettimeofday_mili();
	result = *(thread->time_last_eat);
	pthread_mutex_unlock(thread->mutex[TO_LAST_EAT]);
	return (result);
}

t_status	put_status(t_thread_data *thread, char *color, char *message, char to_die)
{
	t_status	status;

	pthread_mutex_lock(thread->mutex[TO_PUT]);
	if (access_life_flag(thread, READ) == NO_ONE_DIED)
	{
		if (to_die)
			access_life_flag(thread, EDIT);
		printf("%s%ld %d is %s\n%s", color, gettimeofday_mili(), thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}

t_status	philo_eat(t_thread_data *philo)
{
	t_status	status;

	pthread_mutex_lock(philo->mutex[RIGHT_FORK]);
	pthread_mutex_lock(philo->mutex[LEFT_FORK]);
	access_time_last_eat(philo, EDIT);
	status = put_status(philo, GREEN, EAT, 0);
	if (status == SUCCESS)
		xsleep(philo->time[TO_EAT] * 1000);
	pthread_mutex_unlock(philo->mutex[RIGHT_FORK]);
	pthread_mutex_unlock(philo->mutex[LEFT_FORK]);
	return (status);
}

t_status	philo_sleep(t_thread_data *philo)
{
	t_status	status;
	status = put_status(philo, BLUE, SLEEP, 0);
	if (status == SUCCESS)
		xsleep(philo->time[TO_SLEEP] * 1000);
	return (status);
}

t_status	philo_think(t_thread_data *philo)
{
	t_status	status;

	status = put_status(philo, CYAN, THINK, 0);
	return (status);
}

static bool	is_full(t_thread_data *monitor, long pretime_last_eat, long time_last_eat)
{
	if (monitor->times_must_eat != UNSPECIFIED && \
		pretime_last_eat != time_last_eat)
	{
		monitor->times_ate++;
		if (monitor->times_ate >= monitor->times_must_eat)
		{
			put_status(monitor, MAGENTA, FULL, 1);
			return(true);
		}
	}
	return(false);
}

void	*monitor_action(void *data)
{
	t_thread_data	*monitor;
	long			pretime_last_eat;
	long			time_last_eat;
	long			time_now;
	long			time_diff;

	monitor = (t_thread_data *)data;
	pretime_last_eat = access_time_last_eat(monitor, READ);
	if (pretime_last_eat == -1)
		return (data);
	while (1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		if (time_last_eat == -1)
			break ;
		if (is_full(monitor, pretime_last_eat, time_last_eat))
			break ;
		time_now = gettimeofday_mili();
		if (time_now == -1)
			break ;
		time_diff = time_now - time_last_eat;
		//printf("%d, %ld, %ld\n", monitor->order,  time_diff, monitor->time[TO_DIE]);
		if (time_diff >= monitor->time[TO_DIE])
		{
			put_status(monitor, RED, DIE, 1);
			break ;
		}
		//xsleep(time_diff * 1000);
		xsleep(MONITOR_INTERVAL);
		pretime_last_eat = time_last_eat;
	}
	return (data);
}

void	*philo_action(void *data)
{
	t_thread_data	*philo;
	t_thread_data	*monitor;

	philo = (t_thread_data *)data;
	monitor = philo->monitor;
	if (xthread_create(&(monitor->thread_id), &monitor_action, monitor, "for monitor"))
		return(data);
	if (philo->order % 2 == 1)
		xsleep(PHILO_INTERVAL);
	while (1)
	{
		if (philo_eat(philo) == FAIL ||
			philo_sleep(philo) == FAIL ||
			philo_think(philo) == FAIL)
			break ;
	}
	xthread_join(monitor->thread_id, "for monitor");
	return (data);
}
