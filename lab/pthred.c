/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthred.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 21:31:14 by rakiyama          #+#    #+#             */
/*   Updated: 2021/12/29 20:00:41 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//#include <pthread.h>

////スレッドに渡す関数
////引数の中身を出力
//void *thread_func(void *thread_data)
//{
//	int	*num;

//	num = (int *)thread_data;
//	printf("thread_func: %d\n", *num);
//	return (thread_data);
//}

////thread_funcとnumを引数にスレッド作成
////numをインクリメント
////numの中身を出力
//int main(void)
//{
//	int			num;
//	pthread_t	thread_id;

//	num = 0;
//	pthread_create(&thread_id, NULL, &thread_func, &num);//スレッド作成
//	while (num < 50000)
//		num++;
//	printf("main: %d\n", num);
//	pthread_join(thread_id, NULL);
//	return (0);
//}

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;//追加

void *thread_func(void *thread_data)
{
	int	*num;

	num = (int *)thread_data;
	pthread_mutex_lock(&mutex);//追加
	printf("thread_func: %d\n", *num);
	pthread_mutex_unlock(&mutex);//追加
	return (thread_data);
}

int main(void)
{
	int			num;
	pthread_t	thread_id;

	num = 0;
	pthread_mutex_init(&mutex, NULL);//追加
	pthread_create(&thread_id, NULL, &thread_func, &num);
	pthread_mutex_lock(&mutex);//追加
	while (num < 50000)
		num++;
	printf("main: %d\n", num);
	pthread_mutex_unlock(&mutex);//追加
	pthread_join(thread_id, NULL);
	pthread_mutex_destroy(&mutex);//追加
	return (0);
}

////philosopher(哲学者)のスレッドが使う関数群
//void eat()
//{
//	pthread_mutex_lock(right_fork);
//	pthread_mutex_lock(left_fork);
//	/* 食事 */
//	pthread_mutex_unlock(right_fork);
//	pthread_mutex_unlock(left_fork);

//}
//void sleep() { /* 睡眠 */ }
//void think() { /* 思考 */ }

////philosopherのスレッドに渡す関数
//void *philo(void *)
//{
//	while (1)
//	{
//		eat();
//		sleep();
//		think();
//	}
//}

////philosopherのスレッドを(死んでいないか)監視するmonitorスレッドに渡す関数
//void *monitor(void *)
//{
//	if (is_died)
//	{
//		/* 死亡 */
//	}
//}

//int main(void)
//{
//	while (number_of_philo)
//	{
//		pthread_create(philo);
//		pthread_create(monitor);
//	}
//}