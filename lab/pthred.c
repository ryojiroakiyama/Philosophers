/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthred.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama  <rakiyama@student.42tokyo.j      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 21:31:14 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/24 21:31:14 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lab.h"
//mutexがあるとないで、thread_funcの出力内容が変わるので確認

pthread_mutex_t mutex;

void	*thread_func(void *var)
{
	pthread_mutex_lock(&mutex);
	printf("in thread_func:%d\n", *(int *)var);
	pthread_mutex_unlock(&mutex);
	return (var);
}

int	main(void)
{
	int			var;
	pthread_t	thread_id;

	var = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&thread_id, NULL, &thread_func, &var);
	pthread_mutex_lock(&mutex);
	while (var < 50000)
		var++;
	pthread_mutex_unlock(&mutex);
	printf("in main:%d\n", var);
	pthread_join(thread_id, NULL);
	pthread_mutex_destroy(&mutex);
	return (0);
}