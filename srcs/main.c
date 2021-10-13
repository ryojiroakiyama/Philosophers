#include "philo.h"


static bool	is_overflow(unsigned long num, \
		int sign, bool *nonnum_check, long max)
{
	if (0 < num && \
		((sign == 1 && max < num) || \
		(sign == -1 && max < num - 1)))
		return (true);
	else if (*nonnum_check)
		*nonnum_check = false;
	return (false);
}

static size_t	pass_isspace(char *str, size_t i)
{
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	return (i);
}

long	ft_atol(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned long	num;
	int				sign;

	i = 0;
	num = 0;
	sign = 1;
	i = pass_isspace(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	*nonnum_check = true;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		num = num * 10 + (str[i] - '0');
		if (is_overflow(num, sign, nonnum_check, LONG_MAX))
			break ;
		i++;
	}
	i = pass_isspace(str, i);
	if (i == 0 || str[i] != '\0')
		*nonnum_check = true;
	return ((long)num * sign);
}

int	ft_atoi(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned int	num;
	int				sign;

	i = 0;
	num = 0;
	sign = 1;
	i = pass_isspace(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	*nonnum_check = true;
	while (str[i] && ('0' <= str[i] && str[i] <= '9'))
	{
		num = num * 10 + (str[i] - '0');
		if (is_overflow(num, sign, nonnum_check, INT_MAX))
			break ;
		i++;
	}
	i = pass_isspace(str, i);
	if (i == 0 || str[i] != '\0')
		*nonnum_check = true;
	return ((int)num * sign);
}

char	put_error(char *message)
{
	printf("error: %s\n", message);
	return (FAIL);
}

char	put_arg_error(char *message)
{
	printf("error: %s\n", message);
	printf("type and order of arguments required:\n");
	printf("1: number_of_philosophers\n");
	printf("2: time_to_die\n");
	printf("3: time_to_eat\n");
	printf("4: time_to_sleep\n");
	printf("5: number_of_times_each_philosopher_must_eat (is optional)\n");
	return (FAIL);
}

void	put_manage_data(t_manage_data *mdata)
{
	printf("manage_data:\n");
	printf("number_of_philosophers %d\n", mdata->number_of_philosophers);
	printf("time_to_die            %ld\n", mdata->time_to_die);
	printf("time_to_eat            %ld\n", mdata->time_to_eat);
	printf("time_to_sleep          %ld\n", mdata->time_to_sleep);
	printf("time_tobe_satisfied    %ld\n", mdata->time_tobe_satisfied);
}

void	put_thread_data(t_thread_data *tdata)
{
	printf("thread_data:\n");
	printf("order                  %d\n", tdata->order);
	printf("time_to_die            %ld\n", tdata->time_to_die);
	printf("time_to_eat            %ld\n", tdata->time_to_eat);
	printf("time_to_sleep          %ld\n", tdata->time_to_sleep);
	printf("time_last_eat          %ld\n", tdata->time_last_eat);
	printf("time_tobe_satisfied    %ld\n", tdata->time_tobe_satisfied);
	printf("someone_died           %d\n", *(tdata->someone_died));
}

char	set_manage_data_time(char **argv, t_manage_data *mdata)
{
	bool	nonnum_check;

	mdata->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (mdata->number_of_philosophers < 0 || nonnum_check)
		return (put_arg_error(argv[1]));
	mdata->time_to_die = ft_atol(argv[2], &nonnum_check);
	if (mdata->time_to_die < 0 || nonnum_check)
		return (put_arg_error(argv[2]));
	mdata->time_to_eat = ft_atol(argv[3], &nonnum_check);
	if (mdata->time_to_eat < 0 || nonnum_check)
		return (put_arg_error(argv[3]));
	mdata->time_to_sleep = ft_atol(argv[4], &nonnum_check);
	if (mdata->time_to_sleep < 0 || nonnum_check)
		return (put_arg_error(argv[4]));
	if (argv[5])
	{
		mdata->time_tobe_satisfied = ft_atol(argv[5], &nonnum_check);
		if (mdata->time_tobe_satisfied < 0 || nonnum_check)
			return (put_arg_error(argv[5]));
	}
	else
		mdata->time_tobe_satisfied = -1;
	return (SUCCESS);
}

char	set_manage_data(char **argv, t_manage_data *mdata)
{
	if (set_manage_data_time(argv, mdata) == FAIL)
		return (FAIL);
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->forks)
		return (put_error("malloc for forks"));
	mdata->threads = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
	if (!mdata->threads)
		return (put_error("malloc for threads"));
	mdata->someone_died = 0;
	return (SUCCESS);
}

char	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*thread;
	int				thread_cnt;

	thread_cnt = mdata->number_of_philosophers;
	while (thread_cnt--)
	{
		thread = mdata->threads + thread_cnt;
		thread->order = thread_cnt;
		if (thread->order == 0)
			thread->right_fork = mdata->forks + (mdata->number_of_philosophers - 1);
		else
			thread->right_fork = mdata->forks + (thread->order - 1);
		thread->left_fork = mdata->forks + thread->order;
		thread->time_to_die = mdata->time_to_die;
		thread->time_to_eat = mdata->time_to_eat;
		thread->time_to_sleep = mdata->time_to_sleep;
		thread->time_tobe_satisfied = mdata->time_tobe_satisfied;
		thread->someone_died = &(mdata->someone_died);
		pthread_mutex_init(mdata->forks + thread_cnt, NULL);
	}
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

void	*philosopher(void *data)
{
	t_thread_data	*tdata;

	tdata = (t_thread_data *)data;
	if (tdata->order % 2 == 1)
		usleep(200);
	pthread_mutex_lock(tdata->right_fork);
	pthread_mutex_lock(tdata->left_fork);
	printf("philo_order : %d\n", tdata->order);
	// philo_eating();
	pthread_mutex_unlock(tdata->right_fork);
	pthread_mutex_unlock(tdata->left_fork);
	// philo_sleeping();
	// philo_thinking();
	return (data);
}

char	run_thread(t_manage_data *mdata)
{
	t_thread_data	*thread;
	int				thread_cnt;
	long			time_start;

	time_start = gettimeofday_milisecond();
	if (time_start < 0)
		return (FAIL);
	thread_cnt = mdata->number_of_philosophers;
	while (thread_cnt--)
	{
		thread = mdata->threads + thread_cnt;
		thread->time_last_eat = time_start;
		pthread_create(&(thread->thread_id), NULL, &philosopher, thread);
	}
	thread_cnt++;
	while (thread_cnt < mdata->number_of_philosophers)
	{
		thread = mdata->threads + thread_cnt;
		pthread_join(thread->thread_id, NULL);
		pthread_mutex_destroy(mdata->forks + thread_cnt);
		thread_cnt++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_manage_data	mdata;
	char			return_status;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of argument"));
	mdata.forks = NULL;
	mdata.threads = NULL;
	if (set_manage_data(argv, &mdata) == SUCCESS && \
		set_thread_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	free(mdata.threads);
	free(mdata.forks);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (return_status);
}