/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:19:03 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/22 22:37:09 by rakiyama         ###   ########.fr       */
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

//void	do_usleep(useconds_t microseconds)
//{
//	long		endtime;
//	useconds_t	lefttime;

//	endtime = gettimeofday_mili() + microseconds / 1000;
//	while (TRUE)
//	{
//		lefttime = (endtime - gettimeofday_mili()) * 1000;
//		printf("%d\n", lefttime);
//		if (lefttime > 0)
//			usleep(lefttime / 2);
//		else
//			break;
//	}
//}

long	gettimeofday_micro()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("error");
		return (-1);
	}
	return (tv.tv_sec * 1000 * 1000 + tv.tv_usec);
}

void	do_usleep(useconds_t microseconds)
{
	long	endtime;
	long	lefttime;

	endtime = gettimeofday_micro() + microseconds;
	while (TRUE)
	{
		lefttime = endtime - gettimeofday_micro();
		printf("%ld\n", lefttime);
		if (lefttime > 0)
			usleep(lefttime / 2);
		else
			break;
	}
}

int main(void)
{
	long		starttime = gettimeofday_mili();
	useconds_t	sleeptime = 100;
	//usleep(sleeptime);
	//printf("diff: %ld\n", gettimeofday_mili() - starttime);
	//starttime = gettimeofday_mili();
	do_usleep(sleeptime);
	printf("diff: %ld\n", gettimeofday_mili() - starttime);
	return (0);
}
