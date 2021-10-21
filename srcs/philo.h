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
# define NO_ONE_DIED 0
# define SOME_ONE_DIED 1

# define EAT "eating"
# define SLEEP "sleeping"
# define THINK "thinking"
# define DIE "died"

//color settings
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"

typedef struct s_thread_data
{
	int				order;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_tobe_satisfied;
	long			time_last_eat;
	pthread_t		thread_id;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*mutex;
	char			*death_flag;
}	t_thread_data;

typedef struct s_manage_data
{
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_tobe_satisfied;
	t_thread_data	*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex;
	char			death_flag;
}	t_manage_data;

#endif