#include "philo.h"

t_status	set_mdata_num(t_manage_data *mdata, long options[OPTION_NUM])
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
	mdata->life_flag = NO_ONE_DIED;
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
	philo->times_ate = 0;
	philo->times_must_eat = mdata->times_must_eat;
	copy_array(mdata->time, philo->time, TIME_NUM);
	philo->life_flag = &(mdata->life_flag);
	philo->mutex[RIGHT_FORK] = mdata->forks + philo_index;
	philo->mutex[LEFT_FORK] = mdata->forks + ((philo_index + 1) % mdata->philo_num);
	philo->mutex[TO_PUT] = mdata->put;
	philo->mutex[TO_LAST_EAT] = mdata->ate + philo_index;
	philo->mutex[TO_LIFE_FLAG] = mdata->life;
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->monitor = mdata->monitors + philo_index;
}

static void	set_thread_data_monitor(t_thread_data *philo, t_thread_data *monitor)
{
	monitor->order = philo->order;
	monitor->times_ate = philo->times_ate;
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
	//philo_index = 0;
	//while (philo_index < mdata->philo_num)
	//{
	//	if (philo_index % 2 == 1)
	//		printf("%s", GREEN);
	//	else
	//		printf("%s", CYAN);
	//	put_thread_data(mdata->philos + philo_index);
	//	put_thread_data(mdata->monitors + philo_index);
	//	printf("%s", RESET);
	//	philo_index++;
	//}
	//exit(0);
	return (SUCCESS);
}
