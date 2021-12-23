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

pthread_mutex_t	*set_mutex(int size)
{
	pthread_mutex_t	*ans;

	ans = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
	if (!ans)
	{
		put_error("malloc for mutex");
		return(NULL);
	}
	if (handle_mutex(ans, size, wrap_mutex_init) == FAIL)
	{
		put_error("init mutex");
		free(ans);
		return(NULL);
	}
	return(ans);
}

typedef enum e_get_idx_mode
{
	THREADS,
	MUTEXIES,
	MODE_NUM
} t_get_idx_mode;

int	get_idx(t_manage_data *mdata, t_get_idx_mode mode, int content)
{
	if (content == 0)
		return 0;
	if (mode == THREADS)
		return (mdata->threinfo[content - 1][INDEX] + mdata->threinfo[content - 1][SIZE]);
	//else if (mode == MUTEXIES)
	//	return (mdata->threinfo[content][INDEX] + mdata->threinfo[content][SIZE])
	return 0;
}

t_status	set_mdata_memory(t_manage_data *mdata)
{
	mdata->threinfo[PHILOS][INDEX] = get_idx(mdata, THREADS, PHILOS);
	mdata->threinfo[PHILOS][SIZE] = mdata->philo_num;
	mdata->threinfo[MONITORS][INDEX] = get_idx(mdata, THREADS, MONITORS);
	mdata->threinfo[MONITORS][SIZE] = mdata->philo_num;
	mdata->threads = (t_thread_data *)malloc(sizeof(t_thread_data) * get_idx(mdata, THREADS, LAST));
	if (!mdata->threads)
		return (put_error("malloc for threads"));
	mdata->forks = set_mutex(mdata->philo_num);
	if (!mdata->forks)
		return (put_error("set_mdata_memory for fork"));
	mdata->lasteat = set_mutex(mdata->philo_num);
	if (!mdata->lasteat)
		return (put_error("set_mdata_memory for ate"));
	mdata->put = set_mutex(1);
	if (!mdata->put)
		return (put_error("set_mdata_memory for put"));
	mdata->life = set_mutex(1);
	if (!mdata->life)
		return (put_error("set_mdata_memory for life"));
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
	free(mdata->threads);
	free_mutex(mdata->forks, mdata->philo_num);
	free_mutex(mdata->lasteat, mdata->philo_num);
	free_mutex(mdata->put, 1);
	free_mutex(mdata->life, 1);
}
