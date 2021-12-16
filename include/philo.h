#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <libc.h>
#include <stdbool.h>

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

// option
# define UNSPECIFIED -1

// status some one died or not
typedef enum e_life
{
	NO_ONE_DIED,
	SOME_ONE_DIED
}	t_life;

// status fucntions return
typedef enum e_status
{
	SUCCESS,
	FAIL
}	t_status;

// mode of access to the functions threads use
typedef enum e_access
{
	READ,
	EDIT,
	ACCESS_NUM
}	t_access;

typedef enum e_option
{
	NUM_OF_PHILO,
	TIME_TO_DIE,
	TIME_TO_EAT,
	TIME_TO_SLEEP,
	TIMES_PHILO_MUST_EAT,
	OPTION_NUM
}	t_option;

typedef enum e_mutex
{
	RIGHT_FORK,
	LEFT_FORK,
	TO_PUT,
	TO_LAST_EAT,
	TO_LIFE_FLAG,
	MUTEX_NUM
}	t_mutex;

typedef enum e_time
{
	TO_DIE,
	TO_EAT,
	TO_SLEEP,
	LAST_EAT,
	SUM_EAT,
	TIME_NUM
}	t_time;

typedef struct s_thread_data
{
	pthread_t				thread_id;
	int						order;
	int						times_must_eat;
	long					time[TIME_NUM];
	pthread_mutex_t			*mutex[MUTEX_NUM];
	long					*time_last_eat;// = &time[LAST_EAT](it's member)
	t_life					*life_flag;// = &mdata->life_flag
	struct s_thread_data	*monitor;/* monitor->time_last_eat = &time[LAST_EAT](philo's member)
										monitor->life_flag = &mdata->life_flag */
}	t_thread_data;

/*
** Members
**  philos:		philos[i] has data of No.(i+1) philosopher(thread).
**  monitors:	monitors[i] has data of No.(i+1) philosopher's monitor(thread).
**  forks:		mutex for eating action. forks[i] is shared by philos[i] <-> philos[i+1].
**  last_eat:	mutex for access to time_last_eat(each thread's member)
**				last_eat[i] is shared by philos[i] <-> monitors[i].
**  put:		mutex for use to put_status(). is shared by all threads.
**  life:		mutex for access to life_flag. is shared by all threads.
*/
typedef struct s_manage_data
{
	int				philo_num;
	int				times_must_eat;
	long			time[TIME_NUM];
	t_life			life_flag;
	t_thread_data	*philos;
	t_thread_data	*monitors;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	put;
	pthread_mutex_t	life;
}	t_manage_data;

// lib.c
long		ft_atol(char *str, bool *is_valid);
int			ft_atoi(char *str, bool *is_valid);

// put.c
void		put_manage_data(t_manage_data *mdata);
void		put_thread_data(t_thread_data *pdata);
t_status	put_error(char *message);
t_status	put_arg_error(char *message);

// thread
long		gettimeofday_milisecond();
t_life		access_life_flag(t_thread_data *thread, t_access mode);
long		access_time_last_eat(t_thread_data *thread, t_access mode);
t_status	put_status(t_thread_data *thread, char *color, char *message, char to_die);
t_status	philo_eat(t_thread_data *philo);
t_status	philo_sleep(t_thread_data *philo);
t_status	philo_think(t_thread_data *philo);
void		*monitor_action(void *data);
void		*philo_action(void *data);

// main
t_status	set_manage_data(t_manage_data *mdata, long options[OPTION_NUM]);
t_status	set_thread_data(t_manage_data *mdata);
t_status	run_thread(t_manage_data *mdata);

#endif