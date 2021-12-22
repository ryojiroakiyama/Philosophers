#include "philo.h"

static long	gettimeofday_micro()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("error");
		return (-1);
	}
	return (tv.tv_sec * 1000 * 1000 + tv.tv_usec);
}

t_status	do_usleep(useconds_t microseconds)
{
	long	time_now;
	long	time_end;
	long	time_left;

	time_now = gettimeofday_micro();
	if (time_now == -1)
		return (FAIL);
	time_end = time_now + microseconds;
	while (TRUE)
	{
		time_now = gettimeofday_micro();
		if (time_now == -1)
			return (FAIL);
		time_left = time_end - time_now;
		if (time_left > 0)
			usleep(time_left / 2);
		else
			break;
	}
	return(SUCCESS);
}

long	gettimeofday_mili()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		put_error("gettimeofday");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int thre_create
(pthread_t *thread, void *(*start_routine)(void *), void *arg, char *message)
{
	int	ret;

	ret = pthread_create(thread, NULL, start_routine, arg);
	if (ret)
	{
		ft_putstr_fd("error: pthread_create: ", STDERR_FILENO);
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (ret);
}

int thre_join(pthread_t thread, char *message)
{
	int ret;

	ret = pthread_join(thread, NULL);
	if (ret)
	{
		ft_putstr_fd("error: pthread_join: ", STDERR_FILENO);
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (ret);
}
