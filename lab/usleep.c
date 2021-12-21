/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:19:03 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/21 19:21:23 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lab.h"

long	gettimeofday_mili()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("error");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

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

int main(void)
{
	long		starttime = gettimeofday_mili();
	useconds_t	sleeptime = 1000000;
	//usleep(sleeptime);
	//printf("diff: %ld\n", gettimeofday_mili() - starttime);
	//starttime = gettimeofday_mili();
	do_usleep(sleeptime);
	printf("diff: %ld\n", gettimeofday_mili() - starttime);
	return (0);
}