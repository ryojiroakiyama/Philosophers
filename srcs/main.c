#include "philo.h"

void	put_manage_data(t_manage_data *mdata)
{
	printf("manage_data:\n");
	printf("number_of_philosophers %d\n", mdata->number_of_philosophers);
	printf("time_to_die            %ld\n", mdata->time_to_die);
	printf("time_to_eat            %ld\n", mdata->time_to_eat);
	printf("time_to_sleep          %ld\n", mdata->time_to_sleep);
	printf("time_tobe_satisfied    %ld\n", mdata->time_tobe_satisfied);
}

void	put_philo_data(t_philo_data *pdata)
{
	printf("philo_data:\n");
	printf("order                  %d\n", pdata->order);
	printf("time_to_die            %ld\n", pdata->time_to_die);
	printf("time_to_eat            %ld\n", pdata->time_to_eat);
	printf("time_to_sleep          %ld\n", pdata->time_to_sleep);
	printf("time_last_eat          %ld\n", pdata->time_last_eat);
	printf("time_tobe_satisfied    %ld\n", pdata->time_tobe_satisfied);
	printf("death_flag           %d\n", *(pdata->death_flag));
}

static bool	is_overflow(unsigned long num, \
		int sign, bool *nonnum_check, long max)
{
	if (0 < num && \
		((sign == 1 && max < num) || \
		(sign == -1 && max < num - 1)))
		return (true);
	else if (*nonnum_check)
		*nonnum_check = false;
	return (false);
}

static size_t	pass_isspace(char *str, size_t i)
{
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	return (i);
}

long	ft_atol(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned long	num;
	int				sign;

	i = 0;
	num = 0;
	sign = 1;
	i = pass_isspace(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	*nonnum_check = true;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		num = num * 10 + (str[i] - '0');
		if (is_overflow(num, sign, nonnum_check, LONG_MAX))
			break ;
		i++;
	}
	i = pass_isspace(str, i);
	if (i == 0 || str[i] != '\0')
		*nonnum_check = true;
	return ((long)num * sign);
}

int	ft_atoi(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned int	num;
	int				sign;

	i = 0;
	num = 0;
	sign = 1;
	i = pass_isspace(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	*nonnum_check = true;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		num = num * 10 + (str[i] - '0');
		if (is_overflow(num, sign, nonnum_check, INT_MAX))
			break ;
		i++;
	}
	i = pass_isspace(str, i);
	if (i == 0 || str[i] != '\0')
		*nonnum_check = true;
	return ((int)num * sign);
}

char	put_error(char *message)
{
	printf("error: %s\n", message);
	return (FAIL);
}

char	put_arg_error(char *message)
{
	printf("error: %s\n", message);
	printf("type and order of arguments required:\n");
	printf("1: number_of_philosophers\n");
	printf("2: time_to_die\n");
	printf("3: time_to_eat\n");
	printf("4: time_to_sleep\n");
	printf("5: number_of_times_each_philosopher_must_eat (is optional)\n");
	return (FAIL);
}

char	set_manage_data_options(char **argv, t_manage_data *mdata)
{
	bool	nonnum_check;

	mdata->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (mdata->number_of_philosophers < 0 || nonnum_check)
		return (put_arg_error(argv[1]));
	mdata->time_to_die = ft_atol(argv[2], &nonnum_check);
	if (mdata->time_to_die < 0 || nonnum_check)
		return (put_arg_error(argv[2]));
	mdata->time_to_eat = ft_atol(argv[3], &nonnum_check);
	if (mdata->time_to_eat < 0 || nonnum_check)
		return (put_arg_error(argv[3]));
	mdata->time_to_sleep = ft_atol(argv[4], &nonnum_check);
	if (mdata->time_to_sleep < 0 || nonnum_check)
		return (put_arg_error(argv[4]));
	if (argv[5])
	{
		mdata->time_tobe_satisfied = ft_atol(argv[5], &nonnum_check);
		if (mdata->time_tobe_satisfied < 0 || nonnum_check)
			return (put_arg_error(argv[5]));
	}
	else
		mdata->time_tobe_satisfied = -1;
	return (SUCCESS);
}

char	set_manage_data(char **argv, t_manage_data *mdata)
{
	if (set_manage_data_options(argv, mdata) == FAIL)
		return (FAIL);
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->forks)
		return (put_error("malloc for forks"));
	mdata->philos = (t_philo_data *)malloc(sizeof(t_philo_data) * mdata->number_of_philosophers);
	if (!mdata->philos)
		return (put_error("malloc for threads"));
	mdata->death_flag = 0;
	return (SUCCESS);
}

char	set_philo_data(t_manage_data *mdata)
{
	t_philo_data	*thread;
	int				thread_index;

	thread_index = mdata->number_of_philosophers;
	while (thread_index--)
	{
		thread = mdata->philos + thread_index;
		thread->order = thread_index + 1;
		thread->time_to_die = mdata->time_to_die;
		thread->time_to_eat = mdata->time_to_eat;
		thread->time_to_sleep = mdata->time_to_sleep;
		thread->time_tobe_satisfied = mdata->time_tobe_satisfied;
		if (thread_index == 0)
			thread->right_fork = mdata->forks + (mdata->number_of_philosophers - 1);
		else
			thread->right_fork = mdata->forks + (thread_index - 1);
		thread->left_fork = mdata->forks + thread_index;
		thread->put_mutex = &(mdata->put_mutex);
		pthread_mutex_init(&(thread->last_eat_mutex), NULL);
		thread->death_mutex = &(mdata->death_mutex);
		thread->death_flag = &(mdata->death_flag);
		pthread_mutex_init(mdata->forks + thread_index, NULL);
	}
	pthread_mutex_init(&(mdata->put_mutex), NULL);
	pthread_mutex_init(&(mdata->death_mutex), NULL);
	return (SUCCESS);
}

long	gettimeofday_milisecond()
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		put_error("gettimeofday");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// void	survival_confirmation(t_philo_data *pdata)
// {
// 	if (gettimeofday_milisecond() - pdata->time_last_eat > pdata->time_to_die)
// 		*(pdata->death_flag) = SOME_ONE_DIED;
// }

// void	put_status(t_philo_data *pdata, char *color, char *message)
// {
// 	pthread_mutex_lock(pdata->put_mutex);
// 	if (*(pdata->death_flag) == NO_ONE_DIED)
// 	{
// 		survival_confirmation(pdata);
// 		if (*(pdata->death_flag) == NO_ONE_DIED)
// 			printf("%s%ld %d is %s\n%s", color, gettimeofday_milisecond(), pdata->order, message, RESET);
// 		else
// 			printf("%s%ld %d is %s\n%s",RED, gettimeofday_milisecond(), pdata->order, DIE, RESET);
// 	}
// 	pthread_mutex_unlock(pdata->put_mutex);
// }

char	access_death_flag(char *death_flag, t_access mode, pthread_mutex_t *mutex)
{
	char	result;

	pthread_mutex_lock(mutex);
	if (mode == READ)
		result = *death_flag;
	else if (mode == EDIT)
		*death_flag = SOME_ONE_DIED;
	pthread_mutex_unlock(mutex);
	return (result);
}


long	access_time_last_eat(long *time_last_eat, t_access mode, pthread_mutex_t *mutex)
{
	long	result;

	pthread_mutex_lock(mutex);
	if (mode == READ)
		result = *time_last_eat;
	else if (mode == EDIT)
		*time_last_eat = gettimeofday_milisecond();
	pthread_mutex_unlock(mutex);
	return (result);
}

void	put_status(t_philo_data *pdata, char *color, char *message)
{
	static char someone_died;

	pthread_mutex_lock(pdata->put_mutex);
	if (*(pdata->death_flag) == NO_ONE_DIED)
			printf("%s%ld %d is %s\n%s", color, gettimeofday_milisecond(), pdata->order, message, RESET);
	else if (someone_died == 0)
	{
		printf("%s%ld %d is %s\n%s",RED, gettimeofday_milisecond(), pdata->order, DIE, RESET);
		someone_died = 1;
	}
	pthread_mutex_unlock(pdata->put_mutex);
}

void	philo_eat(t_philo_data *pdata)
{
	pdata->time_last_eat = gettimeofday_milisecond();
	put_status(pdata, GREEN, EAT);
	if (*(pdata->death_flag) == NO_ONE_DIED)
		usleep(pdata->time_to_eat * 1000);
}

void	philo_sleep(t_philo_data *pdata)
{
	put_status(pdata, CYAN, SLEEP);
	if (*(pdata->death_flag) == NO_ONE_DIED)
		usleep(pdata->time_to_sleep * 1000);
}

void	philo_think(t_philo_data *pdata)
{
	put_status(pdata, MAGENTA, THINK);
}

void	*monitor(void *data)
{
	t_monitor_data	*monidata;
	long			

	monidata = (t_monitor_data *)data;
	while (1)
	{
		if (gettimeofday_milisecond() - monidata->time_last_eat > monidata->time_to_die)
		{
			*(monidata->death_flag) = SOME_ONE_DIED;
			break ;
		}
		usleep(10000);
	}
}

void	*philosopher(void *data)
{
	t_philo_data	*pdata;
	t_monitor_data	monidata;

	pdata = (t_philo_data *)data;
	monidata.time_to_die = pdata->time_to_die;
	monidata.time_tobe_satisfied = pdata->time_tobe_satisfied;
	monidata.time_last_eat = &(pdata->time_last_eat);
	monidata.last_eat_mutex = &(pdata->last_eat_mutex);
	monidata.death_mutex = pdata->death_mutex;
	monidata.death_flag = pdata->death_flag;
	//pthread_create(&(thread->thread_id), NULL, &philosopher, thread);
	if (pdata->order % 2 == 1)
		usleep(200);//check error?
	while (*(pdata->death_flag) == NO_ONE_DIED)
	{
		pthread_mutex_lock(pdata->right_fork);
		pthread_mutex_lock(pdata->left_fork);
		philo_eat(pdata);
		pthread_mutex_unlock(pdata->right_fork);
		pthread_mutex_unlock(pdata->left_fork);
		philo_sleep(pdata);
		philo_think(pdata);
	}
	//pthread_join(thread->thread_id, NULL);
	return (data);
}

char	run_thread(t_manage_data *mdata)
{
	t_philo_data	*thread;
	int				thread_index;
	long			time_start;

	time_start = gettimeofday_milisecond();//check error?
	if (time_start < 0)
		return (FAIL);
	thread_index = mdata->number_of_philosophers;
	while (thread_index--)
	{
		thread = mdata->philos + thread_index;
		thread->time_last_eat = time_start;
		pthread_create(&(thread->thread_id), NULL, &philosopher, thread);
	}
	while (++thread_index < mdata->number_of_philosophers)
	{
		thread = mdata->philos + thread_index;
		pthread_join(thread->thread_id, NULL);
		pthread_mutex_destroy(&(thread->last_eat_mutex));
		pthread_mutex_destroy(mdata->forks + thread_index);
	}
	pthread_mutex_destroy(&(mdata->put_mutex));
	pthread_mutex_destroy(&(mdata->death_mutex));
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_manage_data	mdata;
	char			return_status;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of argument"));
	mdata.forks = NULL;
	mdata.philos = NULL;
	if (set_manage_data(argv, &mdata) == SUCCESS && \
		set_philo_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	free(mdata.philos);
	free(mdata.forks);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (return_status);
}