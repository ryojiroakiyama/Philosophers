#include "philo.h"

t_status	run_thread(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;
	long			time_start;

	time_start = gettimeofday_milisecond();
	if (time_start < 0)
		return (FAIL);
	philo_index = mdata->philo_num;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		a_philo->time[LAST_EAT] = time_start;
		pthread_create(&(a_philo->thread_id), NULL, &philo_action, a_philo);
	}
	while (++philo_index < mdata->philo_num)
	{
		a_philo = mdata->philos + philo_index;
		pthread_join(a_philo->thread_id, NULL);
		pthread_mutex_destroy(mdata->forks + philo_index);
		pthread_mutex_destroy(mdata->ate + philo_index);
	}
	pthread_mutex_destroy(&(mdata->put));
	pthread_mutex_destroy(&(mdata->life));
	return (SUCCESS);
}

static void	free_memory(t_manage_data *mdata)
{
	if (mdata->philos)
		free(mdata->philos);
	if (mdata->monitors)
		free(mdata->monitors);
	if (mdata->forks)
		free(mdata->forks);
	if (mdata->ate)
		free(mdata->ate);
}

/*
**  argv[1] options[0]: number_of_philosophers, int
**      [2]        [1]: time_to_die, long
**      [3]        [2]: time_to_eat, long
**      [4]        [3]: time_to_sleep, long
**      [5]        [4]: number_of_times_each_philosopher_must_eat, int
*/
static t_status get_options(int argc, char *argv[], long options[OPTION_NUM])
{
	bool	invalid;
	int		idx_arg;
	int		idx_options;

	options[TIMES_PHILO_MUST_EAT] = UNSPECIFIED;
	idx_arg = 1;
	idx_options = 0;
	while (idx_arg < argc)
	{
		if (idx_options % 4)
			options[idx_options] = ft_atol(argv[idx_arg], &invalid);
		else
			options[idx_options] = ft_atoi(argv[idx_arg], &invalid);
		if (options[idx_options] < 0 || invalid)
			return (put_arg_error(argv[idx_arg]));
		idx_arg++;
		idx_options++;
	}
	return(SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_manage_data	mdata;
	t_status		return_status;
	long			options[OPTION_NUM];

	if (!(argc == 5 || argc == 6))
		return (put_arg_error("number of arguments"));
	mdata.philos = NULL;
	mdata.monitors = NULL;
	mdata.forks = NULL;
	mdata.ate = NULL;
	if (get_options(argc, argv, options) == SUCCESS && \
		set_mdata_num(&mdata, options) == SUCCESS && \
		set_mdata_mem(&mdata) == SUCCESS && \
		set_thread_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	free_memory(&mdata);
	return (return_status);
}
