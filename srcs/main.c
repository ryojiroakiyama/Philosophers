#include "philo.h"

static char	set_manage_data_options(t_manage_data *mdata, char **argv)
{
	bool	nonnum_check;

	mdata->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (mdata->number_of_philosophers < 0 || nonnum_check)
		return (put_arg_error(argv[1]));
	mdata->time[TO_DIE] = ft_atol(argv[2], &nonnum_check);
	if (mdata->time[TO_DIE] < 0 || nonnum_check)
		return (put_arg_error(argv[2]));
	mdata->time[TO_EAT] = ft_atol(argv[3], &nonnum_check);
	if (mdata->time[TO_EAT] < 0 || nonnum_check)
		return (put_arg_error(argv[3]));
	mdata->time[TO_SLEEP] = ft_atol(argv[4], &nonnum_check);
	if (mdata->time[TO_SLEEP] < 0 || nonnum_check)
		return (put_arg_error(argv[4]));
	if (argv[5])
	{
		mdata->time[BE_FULL] = ft_atol(argv[5], &nonnum_check);
		if (mdata->time[BE_FULL] < 0 || nonnum_check)
			return (put_arg_error(argv[5]));
	}
	else
		mdata->time[BE_FULL] = LONG_MAX;
	return (SUCCESS);
}

char	set_manage_data(t_manage_data *mdata, char **argv)
{
	if (set_manage_data_options(mdata, argv) == FAIL)
		return (FAIL);
	mdata->death_flag = NO_ONE_DIED;
	mdata->philos = (t_data *)malloc(sizeof(t_data) * mdata->number_of_philosophers);
	if (!mdata->philos)
		return (put_error("malloc for threads"));
	mdata->monitors = (t_data *)malloc(sizeof(t_data) * mdata->number_of_philosophers);
	if (!mdata->monitors)
		return (put_error("malloc for threads"));
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->forks)
		return (put_error("malloc for forks"));
	mdata->last_eat = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->last_eat)
		return (put_error("malloc for last_eat"));
	return (SUCCESS);
}

static void	set_data_philo(t_manage_data *mdata, t_data *philo, int philo_index)
{
	philo->order = philo_index + 1;
	philo->time[TO_DIE] = mdata->time[TO_DIE];
	philo->time[TO_EAT] = mdata->time[TO_EAT];
	philo->time[TO_SLEEP] = mdata->time[TO_SLEEP];
	philo->time[BE_FULL] = mdata->time[BE_FULL];
	philo->time[LAST_EAT] = 0;
	philo->time[SUM_EAT] = 0;
	if (philo_index == 0)
		philo->mutex[RIGHT_FORK] = mdata->forks + (mdata->number_of_philosophers - 1);
	else
		philo->mutex[RIGHT_FORK] = mdata->forks + (philo_index - 1);
	philo->mutex[LEFT_FORK] = mdata->forks + philo_index;
	philo->mutex[TO_PUT] = &(mdata->put);
	philo->mutex[TO_LAST_EAT] = mdata->last_eat + philo_index;
	philo->mutex[TO_DEATH_FLAG] = &(mdata->death);
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->death_flag = &(mdata->death_flag);
	philo->monitor = mdata->monitors + philo_index;
}

static void	set_data_monitor(t_data *philo, t_data *monitor)
{
	monitor->order = philo->order;
	monitor->time[TO_DIE] = philo->time[TO_DIE];
	monitor->time[TO_EAT] = philo->time[TO_EAT];
	monitor->time[TO_SLEEP] = philo->time[TO_SLEEP];
	monitor->time[BE_FULL] = philo->time[BE_FULL];
	monitor->time[LAST_EAT] = 0;
	monitor->time[SUM_EAT] = 0;
	monitor->mutex[RIGHT_FORK] = NULL;
	monitor->mutex[LEFT_FORK] = NULL;
	monitor->mutex[TO_PUT] = philo->mutex[TO_PUT];
	monitor->mutex[TO_LAST_EAT] = philo->mutex[TO_LAST_EAT];
	monitor->mutex[TO_DEATH_FLAG] = philo->mutex[TO_DEATH_FLAG];
	monitor->time_last_eat = philo->time_last_eat;
	monitor->death_flag = philo->death_flag;
	monitor->monitor = NULL;
}

char	set_data(t_manage_data *mdata)
{
	t_data	*a_philo;
	int				philo_index;

	philo_index = mdata->number_of_philosophers;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		set_data_philo(mdata, a_philo, philo_index);
		set_data_monitor(a_philo, a_philo->monitor);
		pthread_mutex_init(mdata->forks + philo_index, NULL);
		pthread_mutex_init(mdata->last_eat + philo_index, NULL);
	}
	pthread_mutex_init(&(mdata->put), NULL);
	pthread_mutex_init(&(mdata->death), NULL);
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

char	access_death_flag(t_data *thread, t_access mode)
{
	char	result;

	pthread_mutex_lock(thread->mutex[TO_DEATH_FLAG]);
	if (mode == EDIT)
		*(thread->death_flag) = SOME_ONE_DIED;
	result = *(thread->death_flag);
	pthread_mutex_unlock(thread->mutex[TO_DEATH_FLAG]);
	return (result);
}

long	access_time_last_eat(t_data *thread, t_access mode)
{
	long	result;

	pthread_mutex_lock(thread->mutex[TO_LAST_EAT]);
	if (mode == EDIT)
 		*(thread->time_last_eat) = gettimeofday_milisecond();
	result = *(thread->time_last_eat);
	pthread_mutex_unlock(thread->mutex[TO_LAST_EAT]);
	return (result);
}

char	put_status(t_data *thread, char *color, char *message, char to_die)
{
	char	status;

	pthread_mutex_lock(thread->mutex[TO_PUT]);
	if (access_death_flag(thread, READ) == NO_ONE_DIED)
	{
		if (to_die)
			access_death_flag(thread, EDIT);
		printf("%s%ld %d is %s\n%s", color, gettimeofday_milisecond(), thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}

char	philo_eat(t_data *philo)
{
	access_time_last_eat(philo, EDIT);
	if (put_status(philo, GREEN, EAT, 0) == SUCCESS)
	{
		usleep(philo->time[TO_EAT] * 1000);
		philo->time[SUM_EAT] += philo->time[TO_EAT];
		if (philo->time[SUM_EAT] >= philo->time[BE_FULL])
		{
			put_status(philo, MAGENTA, FULL, 0);
			return (FAIL);
		}
		return (SUCCESS);
	}
	else
		return (FAIL);
}

char	philo_sleep(t_data *philo)
{
	if (put_status(philo, BLUE, SLEEP, 0) == SUCCESS)
	{
		usleep(philo->time[TO_SLEEP] * 1000);
		return (SUCCESS);
	}
	else
		return (FAIL);
}

char	philo_think(t_data *philo)
{
	if (put_status(philo, CYAN, THINK, 0) == SUCCESS)
		return (SUCCESS);
	else
		return (FAIL);
}

void	*monitor_action(void *data)
{
	t_data	*monitor;
	long			time_last_eat;
	long			time_now;

	monitor = (t_data *)data;
	while (1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		time_now = gettimeofday_milisecond();
		if (time_now - time_last_eat > monitor->time[TO_DIE])
		{
			put_status(monitor, RED, DIE, 1);
			break ;
		}
		usleep(10000);
	}
	return (data);
}

void	*philo_action(void *data)
{
	t_data	*philo;
	t_data	*minimoni;
	char			status;

	philo = (t_data *)data;
	minimoni = philo->monitor;
	pthread_create(&(minimoni->thread_id), NULL, &monitor_action, minimoni);
	if (philo->order % 2 == 1)
		usleep(200);
	status = SUCCESS;
	while (status == SUCCESS)
	{
		pthread_mutex_lock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_lock(philo->mutex[LEFT_FORK]);
		status = philo_eat(philo);
		pthread_mutex_unlock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_unlock(philo->mutex[LEFT_FORK]);
		if (status == SUCCESS)// necessary to end by philo be FULL.
			status =  philo_sleep(philo);
		if (status == SUCCESS)
			status = philo_think(philo);
	}
	pthread_join(minimoni->thread_id, NULL);
	return (data);
}

char	run_thread(t_manage_data *mdata)
{
	t_data	*a_philo;
	int				philo_index;
	long			time_start;

	time_start = gettimeofday_milisecond();
	if (time_start < 0)
		return (FAIL);
	philo_index = mdata->number_of_philosophers;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		a_philo->time[LAST_EAT] = time_start;
		pthread_create(&(a_philo->thread_id), NULL, &philo_action, a_philo);
	}
	while (++philo_index < mdata->number_of_philosophers)
	{
		a_philo = mdata->philos + philo_index;
		pthread_join(a_philo->thread_id, NULL);
		pthread_mutex_destroy(mdata->forks + philo_index);
		pthread_mutex_destroy(mdata->last_eat + philo_index);
	}
	pthread_mutex_destroy(&(mdata->put));
	pthread_mutex_destroy(&(mdata->death));
	return (SUCCESS);
}

static void	handle_memory(t_manage_data *mdata, t_memory mode)
{
	if (mode == INIT)
	{
		mdata->philos = NULL;
		mdata->monitors = NULL;
		mdata->forks = NULL;
		mdata->last_eat = NULL;
	}
	else if (mode == FREE)
	{
		free(mdata->philos);
		free(mdata->monitors);
		free(mdata->forks);
		free(mdata->last_eat);
	}
}

int	main(int argc, char **argv)
{
	t_manage_data	mdata;
	char			return_status;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of arguments"));
	handle_memory(&mdata, INIT);
	if (set_manage_data(&mdata, argv) == SUCCESS && \
		set_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	handle_memory(&mdata, FREE);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (return_status);
}