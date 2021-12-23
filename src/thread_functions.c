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

t_status	put_status(t_thread_data *thread, char *color, char *message, t_put_mode to)
{
	t_status	status;

	pthread_mutex_lock(thread->mutex[TO_PUT]);
	if (access_life_flag(thread, READ) == NO_ONE_DIED)
	{
		if (to == END)
			access_life_flag(thread, EDIT);
		printf("%s%ld %d is %s\n%s", color, gettimeofday_mili(), thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}
