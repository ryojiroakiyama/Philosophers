#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <libc.h>
#include <stdbool.h>

typedef struct s_philo_data
{
	pthread_t		thread_id;
	pthread_mutex_t	*forks;
	int				order;
	int				rfork;
	int				lfork;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_last_eat;
	long			time_tobe_satisfied;
	char			*someone_died;
}	t_thread_data;

typedef struct s_manage_data
{
	t_thread_data	*threads;
	pthread_mutex_t	*forks;
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_tobe_satisfied;
	char			someone_died;
}	t_manage_data;

#endif