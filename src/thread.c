#include "philo.h"

long	gettimeofday_milisecond()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		put_error("gettimeofday");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

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
 		*(thread->time_last_eat) = gettimeofday_milisecond();
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
		printf("%s%ld %d is %s\n%s", color, gettimeofday_milisecond(), thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}

t_status	philo_eat(t_thread_data *philo)
{
	access_time_last_eat(philo, EDIT);
	if (put_status(philo, GREEN, EAT, 0) == SUCCESS)
	{
		usleep(philo->time[TO_EAT] * 1000);
		//philo->time[SUM_EAT] += philo->time[TO_EAT];
		//if (philo->time[SUM_EAT] >= philo->times_must_eat)
		//{
		//	put_status(philo, MAGENTA, FULL, 0);
		//	return (FAIL);
		//}
		return (SUCCESS);
	}
	else
		return (FAIL);
}

t_status	philo_sleep(t_thread_data *philo)
{
	if (put_status(philo, BLUE, SLEEP, 0) == SUCCESS)
	{
		usleep(philo->time[TO_SLEEP] * 1000);
		return (SUCCESS);
	}
	else
		return (FAIL);
}

t_status	philo_think(t_thread_data *philo)
{
	if (put_status(philo, CYAN, THINK, 0) == SUCCESS)
		return (SUCCESS);
	else
		return (FAIL);
}

void	*monitor_action(void *data)
{
	t_thread_data	*monitor;
	long			time_last_eat;
	long			time_now;

	monitor = (t_thread_data *)data;
	while (1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		time_now = gettimeofday_milisecond();
		if (time_now - time_last_eat > monitor->time[TO_DIE])
		{
			put_status(monitor, RED, DIE, 1);
			break ;
		}
		usleep(10000);
	}
	return (data);
}

void	*philo_action(void *data)
{
	t_thread_data	*philo;
	t_thread_data	*minimoni;
	t_status		status;

	philo = (t_thread_data *)data;
	minimoni = philo->monitor;
	pthread_create(&(minimoni->thread_id), NULL, &monitor_action, minimoni);
	if (philo->order % 2 == 1)
		usleep(200);
	status = SUCCESS;
	while (status == SUCCESS)
	{
		pthread_mutex_lock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_lock(philo->mutex[LEFT_FORK]);
		status = philo_eat(philo);
		pthread_mutex_unlock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_unlock(philo->mutex[LEFT_FORK]);
		if (status == SUCCESS)// necessary to end by philo be FULL.
			status =  philo_sleep(philo);
		if (status == SUCCESS)
			status = philo_think(philo);
	}
	pthread_join(minimoni->thread_id, NULL);
	return (data);
}
