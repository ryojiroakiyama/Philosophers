#include "philo.h"

static int	wrap_mutex_init(pthread_mutex_t *mutex)
{
	return pthread_mutex_init(mutex, NULL);
}

static t_status handle_mutex(pthread_mutex_t *mutex, int size, int (*f)(pthread_mutex_t *))
{
	int	idx;

	idx = 0;
	while (idx < size)
	{
		if (f(mutex + idx))
			return (FAIL);
		idx++;
	}
	return (SUCCESS);
}

t_status	set_mdata_mem(t_manage_data *mdata)
{
	mdata->philos = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->philo_num);
	if (!mdata->philos)
		return (put_error("malloc for philo thread"));
	mdata->monitors = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->philo_num);
	if (!mdata->monitors)
		return (put_error("malloc for monitor thread"));
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->philo_num);
	if (mdata->forks)
	{
		memset(mdata->forks, 0, sizeof(pthread_mutex_t) * mdata->philo_num);
		if (handle_mutex(mdata->forks, mdata->philo_num, wrap_mutex_init) == FAIL)
			return (put_error("init forks mutex"));
	}
	else
		return (put_error("malloc for fork mutex"));
	mdata->ate = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->philo_num);
	if (mdata->ate)
	{
		memset(mdata->ate, 0, sizeof(pthread_mutex_t) * mdata->philo_num);
		if (handle_mutex(mdata->ate, mdata->philo_num, wrap_mutex_init) == FAIL)
			return (put_error("init ate mutex"));
	}
	else
		return (put_error("malloc for ate mutex"));
	mdata->put = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * 1);
	if (mdata->put)
	{
		memset(mdata->put, 0, sizeof(pthread_mutex_t) * 1);
		if (handle_mutex(mdata->put, 1, wrap_mutex_init) == FAIL)
			return (put_error("init put mutex"));
	}
	else
		return (put_error("malloc for put mutex"));
	mdata->life = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * 1);
	if (mdata->life)
	{
		memset(mdata->life, 0, sizeof(pthread_mutex_t) * 1);
		if (handle_mutex(mdata->life, 1, wrap_mutex_init) == FAIL)
			return (put_error("init life mutex"));
	}
	else
		return (put_error("malloc for life mutex"));
	return (SUCCESS);
}

static void free_mutex(pthread_mutex_t *m, int size)
{
	if (m)
	{
		if (handle_mutex(m, size, pthread_mutex_destroy) == FAIL)
			put_error("mutex destroy");
		free(m);
	}
}

void	free_memory(t_manage_data *mdata)
{
	free(mdata->philos);
	free(mdata->monitors);
	free_mutex(mdata->forks, mdata->philo_num);
	free_mutex(mdata->ate, mdata->philo_num);
	free_mutex(mdata->put, 1);
	free_mutex(mdata->life, 1);
}
