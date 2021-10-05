#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <libc.h>

typedef struct s_state
{
	pthread_mutex_t	*forks;
	int				order;
	int				time_to_die;
	int				time_from_last_eat;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_tobe_satisfied;
	char			someone_died;
}	t_state;

#endif