#include "philo.h"

long	access_time_last_eat(t_thread_data *thread, t_access_mode mode)
{
	long	result;

	pthread_mutex_lock(thread->mutex[TO_LAST_EAT]);
	if (mode == EDIT)
		*(thread->time_last_eat) = gettimeofday_mili();
	else if (mode == ZERO)
		*(thread->time_last_eat) = 0;
	result = *(thread->time_last_eat);
	pthread_mutex_unlock(thread->mutex[TO_LAST_EAT]);
	return (result);
}

t_status	put_status(t_thread_data *thread, char *color, \
								char *message, t_put_mode to)
{
	t_status	status;

	pthread_mutex_lock(thread->mutex[TO_PUT]);
	if (*thread->is_died == false && *thread->is_full == false)
	{
		if (to == END_DIE || to == END_ERROR)
			*thread->is_died = true;
		else if (to == END_FULL)
			*thread->is_full = true;
		printf("%s%ld %d %s\n%s", color, gettimeofday_mili(), \
									thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}
