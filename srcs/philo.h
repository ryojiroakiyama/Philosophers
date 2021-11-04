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
# define BLUE "\033[34m"
# define CYAN "\033[36m"
# define BLACK "\033[30m"
# define GREEN "\033[32m"
# define WHITE "\033[37m"
# define YEELOW "\033[33m"
# define MAGENTA "\033[35m"
# define RESET "\033[0m"

typedef enum e_access
{
	READ,
	EDIT
}	t_access;

typedef struct s_monitor_data
{
	long			time_to_die;
	long			time_tobe_satisfied;
	long			*time_last_eat;
	pthread_t		thread_id;
	pthread_mutex_t	*last_eat_mutex;
	pthread_mutex_t	*death_mutex;
	char			*death_flag;
}	t_monitor_data;

typedef struct s_philo_data
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
	pthread_mutex_t	*put_mutex;
	pthread_mutex_t	last_eat_mutex;
	pthread_mutex_t	*death_mutex;
	char			*death_flag;
}	t_philo_data;

typedef struct s_manage_data
{
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_tobe_satisfied;
	t_philo_data	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	put_mutex;
	pthread_mutex_t	death_mutex;
	char			death_flag;
}	t_manage_data;

#endif