#include "philo.h"

static void	set_options(t_manage_data *mdata, long options[OPTION_NUM])
{
	t_time	idx_time;

	mdata->philo_num = options[NUM_OF_PHILO];
	mdata->times_must_eat = options[TIMES_PHILO_MUST_EAT];
	idx_time = 0;
	while (idx_time < TIME_NUM)
	{
		mdata->time[idx_time] = 0;
		if (idx_time <= TO_SLEEP)
			mdata->time[idx_time] = options[idx_time + 1];
		idx_time++;
	}
}

t_status	set_manage_data(t_manage_data *mdata, long options[OPTION_NUM])
{
	set_options(mdata, options);
	mdata->life_flag = NO_ONE_DIED;
	mdata->philos = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->philo_num);
	if (!mdata->philos)
		return (put_error("malloc for philo thread"));
	mdata->monitors = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->philo_num);
	if (!mdata->monitors)
		return (put_error("malloc for monitor thread"));
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->philo_num);
	if (!mdata->forks)
		return (put_error("malloc for fork mutex"));
	mdata->ate = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->philo_num);
	if (!mdata->ate)
		return (put_error("malloc for ate mutex"));
	return (SUCCESS);
}

static void copy_array(long original[], long copy[], int size)
{
	int	idx;

	idx = 0;
	while (idx < size)
	{
		copy[idx] = original[idx];
		idx++;
	}
}

static void	set_thread_data_philo(t_manage_data *mdata, t_thread_data *philo, int philo_index)
{
	philo->order = philo_index + 1;
	philo->times_must_eat = mdata->times_must_eat;
	copy_array(mdata->time, philo->time, TIME_NUM);
	philo->life_flag = &(mdata->life_flag);
	philo->mutex[RIGHT_FORK] = mdata->forks + philo_index;
	philo->mutex[LEFT_FORK] = mdata->forks + ((philo_index + 1) % mdata->philo_num);
	philo->mutex[TO_PUT] = &(mdata->put);
	philo->mutex[TO_LAST_EAT] = mdata->ate + philo_index;
	philo->mutex[TO_LIFE_FLAG] = &(mdata->life);
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->monitor = mdata->monitors + philo_index;
}

static void	set_thread_data_monitor(t_thread_data *philo, t_thread_data *monitor)
{
	monitor->order = philo->order;
	monitor->times_must_eat = philo->times_must_eat;
	copy_array(philo->time, monitor->time, TIME_NUM);
	monitor->mutex[RIGHT_FORK] = NULL;
	monitor->mutex[LEFT_FORK] = NULL;
	monitor->mutex[TO_PUT] = philo->mutex[TO_PUT];
	monitor->mutex[TO_LAST_EAT] = philo->mutex[TO_LAST_EAT];
	monitor->mutex[TO_LIFE_FLAG] = philo->mutex[TO_LIFE_FLAG];
	monitor->time_last_eat = philo->time_last_eat;
	monitor->life_flag = philo->life_flag;
	monitor->monitor = NULL;
}

static t_status init_mutex(pthread_mutex_t *mutex, int len)
{
	int	idx;

	idx = 0;
	while (idx < len)
	{
		if (pthread_mutex_init(mutex + idx, NULL))
			return (FAIL);
		idx++;
	}
	return (SUCCESS);
}

t_status	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;

	philo_index = mdata->philo_num;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		set_thread_data_philo(mdata, a_philo, philo_index);
		set_thread_data_monitor(a_philo, a_philo->monitor);
	}
	memset(mdata->forks, 0, sizeof(pthread_mutex_t) * mdata->philo_num);
	init_mutex(mdata->forks, mdata->philo_num);
	memset(mdata->ate, 0, sizeof(pthread_mutex_t) * mdata->philo_num);
	init_mutex(mdata->ate, mdata->philo_num);
	memset(&(mdata->put), 0, sizeof(pthread_mutex_t));
	init_mutex(&(mdata->put), 1);
	memset(&(mdata->life), 0, sizeof(pthread_mutex_t));
	init_mutex(&(mdata->life), 1);
	//philo_index = 0;
	//while (philo_index < 5)
	//{
	//	put_thread_data(mdata->philos + philo_index);
	//	philo_index++;
	//}
	//exit(0);
	return (SUCCESS);
}
