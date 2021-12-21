#include "philo.h"

void	do_usleep(useconds_t microseconds)
{
	long		endtime;
	useconds_t	lefttime;

	endtime = gettimeofday_mili() + microseconds / 1000;
	while (TRUE)
	{
		lefttime = (endtime - gettimeofday_mili()) * 1000;
		if (lefttime > 0)
			usleep(lefttime / 2);
		else
			break;
	}
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

int xthread_create
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

int xthread_join(pthread_t thread, char *message)
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
