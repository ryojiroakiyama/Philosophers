#include "philo.h"

// for handle memory
typedef enum e_memory
{
	INIT,
	FREE,
	MEMORY_NUM
}	t_memory;

t_status	run_thread(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
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
	pthread_mutex_destroy(&(mdata->life));
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
	t_status		return_status;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of arguments"));
	handle_memory(&mdata, INIT);
	if (set_manage_data(&mdata, argv) == SUCCESS && \
		set_thread_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	handle_memory(&mdata, FREE);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (return_status);
}
