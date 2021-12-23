#include "philo.h"

t_status	run_thread(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;
	long			time_start;

	time_start = gettimeofday_mili();
	if (time_start == -1)
		return (FAIL);
	philo_index = mdata->philo_num;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		a_philo->time[LAST_EAT] = time_start;
		thre_create(&(a_philo->thread_id), &philo_action, a_philo, "for philo");
	}
	while (++philo_index < mdata->philo_num)
	{
		a_philo = mdata->philos + philo_index;
		thre_join(a_philo->thread_id, "for philo");
	}
	return (SUCCESS);
}
