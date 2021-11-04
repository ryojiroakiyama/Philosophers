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
	PUT,
	MEAL,
	DEATH,
	MUTEX_NUM
}	t_mutex;

typedef enum e_time
{
	TO_DIE,
	TO_EAT,
	TO_SLEEP,
	BE_STUFFED,
	LAST_EAT,
	TIME_NUM
}	t_time;

typedef struct s_thread_data
{
	pthread_t				thread_id;
	int						order;
	long					time[TIME_NUM];
	long					*time_last_eat;// = &time[LAST_EAT]
	char					*death_flag;// = &mdata->death_flag
	pthread_mutex_t			*mutex[MUTEX_NUM];
	struct s_thread_data	*monitor;// monitor->time_last_eat = &time[LAST_EAT]
	//								   monitor->death_flag = &mdata->death_flag
}	t_thread_data;

/*
** Members as pointer type
** (Memory will be allocated for
** sizeof(type) x (number of philosophers)).
**   philos : philos[0] has data of first philosopher's thread.
**   times_last_eat : times_last_eat[0] is shared by philos[0] <-> monitors[0].
**   forks : forks[0] is shared by philos[0] <-> philos[1].
**   last_eat : last_eat[0] is used to lock access to times_last_eat[0].
**
** Else members
**   time : Values of arguments.
**   death_flag : shared by all threads.
**   put : put is used to lock use of put_status().
**   death : death is used to lock access to death_flag.
*/
typedef struct s_manage_data
{
	int				number_of_philosophers;
	long			time[TIME_NUM];
	char			death_flag;
	t_thread_data	*philos;
	t_thread_data	*monitors;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	put;
	pthread_mutex_t	death;
}	t_manage_data;

#endif