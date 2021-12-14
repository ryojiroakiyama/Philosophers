#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <libc.h>
#include <stdbool.h>

// status
# define SUCCESS 0
# define FAIL 1

// flag some one died or not
# define NO_ONE_DIED 0
# define SOME_ONE_DIED 1

// message to put there status
# define EAT "eating"
# define SLEEP "sleeping"
# define THINK "thinking"
# define DIE "died"
# define FULL "is full"

// color settings
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
	EDIT,
	ACCESS_NUM
}	t_access;

typedef enum e_memory
{
	INIT,
	FREE,
	MEMORY_NUM
}	t_memory;

typedef enum e_mutex
{
	RIGHT_FORK,
	LEFT_FORK,
	TO_PUT,
	TO_LAST_EAT,
	TO_DEATH_FLAG,
	MUTEX_NUM
}	t_mutex;

typedef enum e_time
{
	TO_DIE,
	TO_EAT,
	TO_SLEEP,
	BE_FULL,
	LAST_EAT,
	SUM_EAT,
	TIME_NUM
}	t_time;

typedef struct s_data
{
	pthread_t		thread_id;
	int				order;
	long			time[TIME_NUM];
	pthread_mutex_t	*mutex[MUTEX_NUM];
	long			*time_last_eat;// = &time[LAST_EAT](it's member)
	char			*death_flag;// = &mdata->death_flag
	struct s_data	*monitor;/* monitor->time_last_eat = &time[LAST_EAT](philo's member)
								monitor->death_flag = &mdata->death_flag */
}	t_data;

/*
** Members
**  philos:		philos[i] has data of No.(i+1) philosopher(thread).
**  monitors:	monitors[i] has data of No.(i+1) philosopher's monitor(thread).
**  forks:		mutex for eating action. forks[i] is shared by philos[i] <-> philos[i+1].
**  last_eat:	mutex for access to time_last_eat(each thread's member)
**				last_eat[i] is shared by philos[i] <-> monitors[i].
**  put:		mutex for use to put_status(). is shared by all threads.
**  death:		mutex for access to death_flag. is shared by all threads.
**  death_flag:	flag indicate no one died or not. 
*/
typedef struct s_manage_data
{
	int				number_of_philosophers;
	long			time[TIME_NUM];
	t_data			*philos;
	t_data			*monitors;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	put;
	pthread_mutex_t	death;
	char			death_flag;
}	t_manage_data;

#endif