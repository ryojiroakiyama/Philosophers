#include "philo.h"

static t_status	handle_mutex(pthread_mutex_t *mutex, int size, \
									int (*f)(pthread_mutex_t *))
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

static int	wrap_mutex_init(pthread_mutex_t *mutex)
{
	return (pthread_mutex_init(mutex, NULL));
}

typedef enum e_get_idx_mode
{
	THREADS,
	MUTEXIES,
	GET_IDX_MODE_NUM
}	t_get_idx_mode;

static int	get_idx(t_manage_data *mdata, t_get_idx_mode mode, int content)
{
	if (content == 0)
		return (0);
	if (mode == THREADS)
		return (mdata->threinfo[content - 1][INDEX] + \
				mdata->threinfo[content - 1][SIZE]);
	else if (mode == MUTEXIES)
		return (mdata->mutexinfo[content - 1][INDEX] + \
				mdata->mutexinfo[content - 1][SIZE]);
	return (0);
}

t_status	set_mdata_memory(t_manage_data *mdata)
{
	int	idx;

	mdata->threinfo[PHILOS][SIZE] = mdata->philo_num;
	mdata->threinfo[MONITORS][SIZE] = mdata->philo_num;
	idx = -1;
	while (++idx < THREADS_NUM)
		mdata->threinfo[idx][INDEX] = get_idx(mdata, THREADS, idx);
	mdata->threads = (t_thread_data *)malloc(sizeof(t_thread_data) * \
									get_idx(mdata, THREADS, THREADS_NUM));
	if (!mdata->threads)
		return (put_error("malloc for threads"));
	mdata->mutexinfo[FORKS][SIZE] = mdata->philo_num;
	mdata->mutexinfo[LASTEATS][SIZE] = mdata->philo_num;
	mdata->mutexinfo[PUTS][SIZE] = 1;
	idx = -1;
	while (++idx < MUTEXIES_NUM)
		mdata->mutexinfo[idx][INDEX] = get_idx(mdata, MUTEXIES, idx);
	mdata->mutexies = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * \
									get_idx(mdata, MUTEXIES, MUTEXIES_NUM));
	if (!mdata->mutexies)
		return (put_error("malloc for mutexies"));
	if (handle_mutex(mdata->mutexies, get_idx(mdata, MUTEXIES, MUTEXIES_NUM), \
													wrap_mutex_init) == FAIL)
		return (put_error("init mutexies"));
	return (SUCCESS);
}

void	free_mdata_memory(t_manage_data *mdata)
{
	free(mdata->threads);
	if (mdata->mutexies)
	{
		if (handle_mutex(mdata->mutexies, \
			get_idx(mdata, MUTEXIES, MUTEXIES_NUM), \
			pthread_mutex_destroy) == FAIL)
			put_error("mutex destroy");
		free(mdata->mutexies);
	}
}
