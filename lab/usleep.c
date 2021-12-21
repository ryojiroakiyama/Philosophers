/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:19:03 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/21 12:05:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lab.h"

int main(void)
{
	printf("before usleep\n");
	usleep(1000000);
	printf("after usleep\n");
	return (0);
}

//#include <unistd.h>
//#include <sys/time.h>

//useconds_t get_time(void) {
//        struct timeval tv;

//        gettimeofday(&tv, NULL);
//        return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
//}

//void Usleep(useconds_t microseconds) {
//        useconds_t endtime, lefttime;

//        endtime = get_time() + microseconds;
//        while ((lefttime = endtime - get_time()) > 0) {
//                usleep(lefttime / 2);
//        }
//}