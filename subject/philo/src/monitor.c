#include "philo.h"

static bool	is_full(t_thread_data *monitor, long pretime_last_eat, \
												long time_last_eat)
{
	if (monitor->times_must_eat != UNSPECIFIED && \
		pretime_last_eat != time_last_eat)
	{
		monitor->times_ate++;
		if (monitor->times_ate >= monitor->times_must_eat)
		{
			put_status(monitor, MAGENTA, FULL, END_FULL);
			return (true);
		}
	}
	return (false);
}

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
	if (do_usleep(time_diff / 2) == FAIL)
		return (true);
	return (false);
}

void	*monitor_action(void *data)
{
	t_thread_data	*monitor;
	long			pretime_last_eat;
	long			time_last_eat;

	monitor = (t_thread_data *)data;
	pretime_last_eat = access_time_last_eat(monitor, READ);
	while (pretime_last_eat != -1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		if (time_last_eat == -1)
			break ;
		if (is_full(monitor, pretime_last_eat, time_last_eat))
			break ;
		if (is_died(monitor, time_last_eat))
			break ;
		pretime_last_eat = time_last_eat;
	}
	put_status(monitor, YEELOW, ERR, END_ERROR);
	return (data);
}
