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

char	set_manage_data_time(char **argv, t_manage_data *mdata)
{
	bool	nonnum_check;

	mdata->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (mdata->number_of_philosophers < 0 || nonnum_check)
		return (1);
	mdata->time_to_die = ft_atol(argv[2], &nonnum_check);
	if (mdata->time_to_die < 0 || nonnum_check)
		return (1);
	mdata->time_to_eat = ft_atol(argv[3], &nonnum_check);
	if (mdata->time_to_eat < 0 || nonnum_check)
		return (1);
	mdata->time_to_sleep = ft_atol(argv[4], &nonnum_check);
	if (mdata->time_to_sleep < 0 || nonnum_check)
		return (1);
	if (argv[5])
	{
		mdata->time_tobe_satisfied = ft_atol(argv[5], &nonnum_check);
		if (mdata->time_tobe_satisfied < 0 || nonnum_check)
			return (1);
	}
	else
		mdata->time_tobe_satisfied = -1;
	return (0);
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
	return (1);
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
	printf("rfork                  %d\n", tdata->rfork);
	printf("lfork                  %d\n", tdata->lfork);
	printf("time_to_die            %ld\n", tdata->time_to_die);
	printf("time_to_eat            %ld\n", tdata->time_to_eat);
	printf("time_to_sleep          %ld\n", tdata->time_to_sleep);
	printf("time_last_eat          %ld\n", tdata->time_last_eat);
	printf("time_tobe_satisfied    %ld\n", tdata->time_tobe_satisfied);
	printf("someone_died           %d\n", *(tdata->someone_died));
}

char	put_error(char *message)
{
	printf("error: %s\n", message);
	return (1);
}

char	set_manage_data(t_manage_data *mdata)
{
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->forks)
		return (put_error("malloc for forks"));
	mdata->threads = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
	if (!mdata->threads)
	{
		free(mdata->forks);
		return (put_error("malloc for threads"));
	}
	mdata->someone_died = 0;
	return (0);
}

char	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*thread;
	int				thread_cnt;

	thread_cnt = mdata->number_of_philosophers;
	while (thread_cnt--)
	{
		thread = mdata->threads + thread_cnt;
		thread->forks = mdata->forks;
		thread->order = thread_cnt;
		if (thread->order == 0)
			thread->rfork = mdata->number_of_philosophers - 1;
		else
			thread->rfork = thread->order - 1;
		thread->lfork = thread->order;
		thread->time_to_die = mdata->time_to_die;
		thread->time_to_eat = mdata->time_to_eat;
		thread->time_to_sleep = mdata->time_to_sleep;
		thread->time_tobe_satisfied = mdata->time_tobe_satisfied;
		thread->someone_died = &(mdata->someone_died);
		pthread_mutex_init(mdata->forks + thread_cnt, NULL);
	}
	return (0);
}

void	*philosopher(void *data)
{
	struct timeval 	tv;
	t_thread_data	*tdata;

	if (gettimeofday(&tv, NULL) == -1)
	{
		put_error("gettimeofday");
		return (0);
	}
	tdata = (t_thread_data *)data;
	tdata->time_last_eat = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	pthread_mutex_lock(tdata->forks + tdata->rfork);
	pthread_mutex_lock(tdata->forks + tdata->lfork);
	put_thread_data(tdata);
	pthread_mutex_unlock(tdata->forks + tdata->rfork);
	pthread_mutex_unlock(tdata->forks + tdata->lfork);
	return (0);
}

char	run_thread(t_manage_data *mdata)
{
	t_thread_data	*thread;
	int				thread_cnt;

	thread_cnt = mdata->number_of_philosophers;
	while (thread_cnt--)
	{
		thread = mdata->threads + thread_cnt;
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
	return (0);
}

int	main(int argc, char **argv)
{
	t_manage_data	mdata;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of argument"));
	if (set_manage_data_time(argv, &mdata))
		return (put_arg_error("type of argument"));
	if (set_manage_data(&mdata))
		return (1);
	if (set_thread_data(&mdata))
		return (1);
	// put_manage_data(&mdata);
	// exit(0);
	if (run_thread(&mdata))
		return (1);
	free(mdata.threads);
	free(mdata.forks);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (0);
}
//片岡さんはモニターを一つ作って、そいつでそれぞれ最後に食べた時間から考えてどれか死んだかを計算して考えてるらしい