#include "philo.h"

static t_status	set_manage_data_options(t_manage_data *mdata, char **argv)
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

t_status	set_manage_data(t_manage_data *mdata, char **argv)
{
	if (set_manage_data_options(mdata, argv) == FAIL)
		return (FAIL);
	mdata->life_flag = NO_ONE_DIED;
	mdata->philos = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
	if (!mdata->philos)
		return (put_error("malloc for threads"));
	mdata->monitors = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
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

static void	set_thread_data_philo(t_manage_data *mdata, t_thread_data *philo, int philo_index)
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
	philo->mutex[TO_LIFE_FLAG] = &(mdata->life);
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->life_flag = &(mdata->life_flag);
	philo->monitor = mdata->monitors + philo_index;
}

static void	set_thread_data_monitor(t_thread_data *philo, t_thread_data *monitor)
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
	monitor->mutex[TO_LIFE_FLAG] = philo->mutex[TO_LIFE_FLAG];
	monitor->time_last_eat = philo->time_last_eat;
	monitor->life_flag = philo->life_flag;
	monitor->monitor = NULL;
}

t_status	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;

	philo_index = mdata->number_of_philosophers;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		set_thread_data_philo(mdata, a_philo, philo_index);
		set_thread_data_monitor(a_philo, a_philo->monitor);
		pthread_mutex_init(mdata->forks + philo_index, NULL);
		pthread_mutex_init(mdata->last_eat + philo_index, NULL);
	}
	pthread_mutex_init(&(mdata->put), NULL);
	pthread_mutex_init(&(mdata->life), NULL);
	return (SUCCESS);
}
