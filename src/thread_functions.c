#include "philo.h"

long	access_time_last_eat(t_thread_data *thread, t_access_mode mode)
{
	long	result;

	pthread_mutex_lock(thread->mutex[TO_LAST_EAT]);
	if (mode == EDIT)
		*(thread->time_last_eat) = gettimeofday_mili();
	result = *(thread->time_last_eat);
	pthread_mutex_unlock(thread->mutex[TO_LAST_EAT]);
	return (result);
}

t_status	put_status(t_thread_data *thread, char *color, \
								char *message, t_put_mode to)
{
	t_status	status;

	pthread_mutex_lock(thread->mutex[TO_LIFE_FLAG]);
	if (*thread->life_flag == NO_ONE_DIED)
	{
		if (to == END)
			*thread->life_flag = SOME_ONE_DIED;
		printf("%s%ld %d is %s\n%s", color, gettimeofday_mili(), \
									thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_LIFE_FLAG]);
	return (status);
}
