#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <libc.h>
#include <stdbool.h>

# define SUCCESS 0
# define FAIL 1

typedef struct s_thread_data
{
	pthread_t		thread_id;
	int				order;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_last_eat;
	long			time_tobe_satisfied;
	char			*death_flag;
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
	char			death_flag;
}	t_manage_data;

#endif