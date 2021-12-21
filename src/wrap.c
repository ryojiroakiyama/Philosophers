#include "philo.h"

void	do_usleep(useconds_t microseconds)
{
	long		endtime;
	useconds_t	lefttime;

	endtime = getmilitimeofday() + microseconds / 1000;
	while (TRUE)
	{
		lefttime = (endtime - getmilitimeofday()) * 1000;
		if (lefttime > 0)
			usleep(lefttime / 2);
		else
			break;
	}
}

long	getmilitimeofday()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		put_error("gettimeofday");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
