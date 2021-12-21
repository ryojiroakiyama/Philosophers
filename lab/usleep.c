/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:19:03 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/21 17:28:57 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lab.h"

long	getmilitimeofday()
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

int main(void)
{
	long		starttime = getmilitimeofday();
	useconds_t	sleeptime = 1000000;
	//usleep(sleeptime);
	//printf("diff: %ld\n", getmilitimeofday() - starttime);
	//starttime = getmilitimeofday();
	do_usleep(sleeptime);
	printf("diff: %ld\n", getmilitimeofday() - starttime);
	return (0);
}