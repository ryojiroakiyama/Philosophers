#include "philo.h"

void	put_manage_data(t_manage_data *mdata)
{
	printf("manage_data:\n");
	printf("number_of_philosophers %d\n", mdata->number_of_philosophers);
	printf("time[TO_DIE]           %ld\n", mdata->time[TO_DIE]);
	printf("time[TO_EAT]           %ld\n", mdata->time[TO_EAT]);
	printf("time[TO_SLEEP]         %ld\n", mdata->time[TO_SLEEP]);
	printf("time[BE_STUFFED]       %ld\n", mdata->time[BE_STUFFED]);
}

void	put_thread_data(t_thread_data *pdata)
{
	printf("philo_data:\n");
	printf("order            %d\n", pdata->order);
	printf("time[TO_DIE]     %ld\n", pdata->time[TO_DIE]);
	printf("time[TO_EAT]     %ld\n", pdata->time[TO_EAT]);
	printf("time[TO_SLEEP]   %ld\n", pdata->time[TO_SLEEP]);
	printf("time[BE_STUFFED] %ld\n", pdata->time[BE_STUFFED]);
	printf("time[LAST_EAT    %ld\n", pdata->time[LAST_EAT]);
	printf("time_last_eat    %ld\n", *(pdata->time_last_eat));
	printf("death_flag       %d\n", *(pdata->death_flag));
}

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

static char	set_manage_data_options(t_manage_data *mdata, char **argv)
{
	bool	nonnum_check;

	mdata->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (mdata->number_of_philosophers < 0 || nonnum_check)
		return (put_arg_error(argv[1]));
	mdata->time[TO_DIE] = ft_atol(argv[2], &nonnum_check);
	if (mdata->time[TO_DIE] < 0 || nonnum_check)
		return (put_arg_error(argv[2]));
	mdata->time[TO_EAT] = ft_atol(argv[3], &nonnum_check);
	if (mdata->time[TO_EAT] < 0 || nonnum_check)
		return (put_arg_error(argv[3]));
	mdata->time[TO_SLEEP] = ft_atol(argv[4], &nonnum_check);
	if (mdata->time[TO_SLEEP] < 0 || nonnum_check)
		return (put_arg_error(argv[4]));
	if (argv[5])
	{
		mdata->time[BE_STUFFED] = ft_atol(argv[5], &nonnum_check);
		if (mdata->time[BE_STUFFED] < 0 || nonnum_check)
			return (put_arg_error(argv[5]));
	}
	else
		mdata->time[BE_STUFFED] = -1;
	return (SUCCESS);
}

char	set_manage_data(t_manage_data *mdata, char **argv)
{
	if (set_manage_data_options(mdata, argv) == FAIL)
		return (FAIL);
	mdata->death_flag = NO_ONE_DIED;
	mdata->philos = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
	if (!mdata->philos)
		return (put_error("malloc for threads"));
	mdata->monitors = (t_thread_data *)malloc(sizeof(t_thread_data) * mdata->number_of_philosophers);
	if (!mdata->monitors)
		return (put_error("malloc for threads"));
	mdata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->forks)
		return (put_error("malloc for forks"));
	mdata->last_eat = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * mdata->number_of_philosophers);
	if (!mdata->last_eat)
		return (put_error("malloc for last_eat"));
	return (SUCCESS);
}

static void	set_thread_data_philo(t_manage_data *mdata, t_thread_data *philo, int philo_index)
{
	philo->order = philo_index + 1;
	philo->time[TO_DIE] = mdata->time[TO_DIE];
	philo->time[TO_EAT] = mdata->time[TO_EAT];
	philo->time[TO_SLEEP] = mdata->time[TO_SLEEP];
	philo->time[BE_STUFFED] = mdata->time[BE_STUFFED];
	philo->time[LAST_EAT] = 0;
	if (philo_index == 0)
		philo->mutex[RIGHT_FORK] = mdata->forks + (mdata->number_of_philosophers - 1);
	else
		philo->mutex[RIGHT_FORK] = mdata->forks + (philo_index - 1);
	philo->mutex[LEFT_FORK] = mdata->forks + philo_index;
	philo->mutex[TO_PUT] = &(mdata->put);
	philo->mutex[TO_LAST_EAT] = mdata->last_eat + philo_index;
	philo->mutex[TO_DEATH_FLAG] = &(mdata->death);
	philo->time_last_eat = &(philo->time[LAST_EAT]);
	philo->death_flag = &(mdata->death_flag);
	philo->monitor = mdata->monitors + philo_index;
}

static void	set_thread_data_monitor(t_thread_data *philo, t_thread_data *monitor)
{
	monitor->order = philo->order;
	monitor->time[TO_DIE] = philo->time[TO_DIE];
	monitor->time[TO_EAT] = philo->time[TO_EAT];
	monitor->time[TO_SLEEP] = philo->time[TO_SLEEP];
	monitor->time[BE_STUFFED] = philo->time[BE_STUFFED];
	monitor->time[LAST_EAT] = 0;
	monitor->mutex[RIGHT_FORK] = NULL;
	monitor->mutex[LEFT_FORK] = NULL;
	monitor->mutex[TO_PUT] = philo->mutex[TO_PUT];
	monitor->mutex[TO_LAST_EAT] = philo->mutex[TO_LAST_EAT];
	monitor->mutex[TO_DEATH_FLAG] = philo->mutex[TO_DEATH_FLAG];
	monitor->time_last_eat = philo->time_last_eat;
	monitor->death_flag = philo->death_flag;
	monitor->monitor = NULL;
}

char	set_thread_data(t_manage_data *mdata)
{
	t_thread_data	*a_philo;
	int				philo_index;

	philo_index = mdata->number_of_philosophers;
	while (philo_index--)
	{
		a_philo = mdata->philos + philo_index;
		set_thread_data_philo(mdata, a_philo, philo_index);
		set_thread_data_monitor(a_philo, a_philo->monitor);
		pthread_mutex_init(mdata->forks + philo_index, NULL);
		pthread_mutex_init(mdata->last_eat + philo_index, NULL);
	}
	pthread_mutex_init(&(mdata->put), NULL);
	pthread_mutex_init(&(mdata->death), NULL);
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

char	access_death_flag(t_thread_data *thread, t_access mode)
{
	char	result;

	pthread_mutex_lock(thread->mutex[TO_DEATH_FLAG]);
	if (mode == EDIT)
		*(thread->death_flag) = SOME_ONE_DIED;
	result = *(thread->death_flag);
	pthread_mutex_unlock(thread->mutex[TO_DEATH_FLAG]);
	return (result);
}

long	access_time_last_eat(t_thread_data *thread, t_access mode)
{
	long	result;

	pthread_mutex_lock(thread->mutex[TO_LAST_EAT]);
	if (mode == EDIT)
 		*(thread->time_last_eat) = gettimeofday_milisecond();
	result = *(thread->time_last_eat);
	pthread_mutex_unlock(thread->mutex[TO_LAST_EAT]);
	return (result);
}

char	put_status(t_thread_data *thread, char *color, char *message, char to_die)
{
	char	status;

	pthread_mutex_lock(thread->mutex[TO_PUT]);
	if (access_death_flag(thread, READ) == NO_ONE_DIED)
	{
		if (to_die)
			access_death_flag(thread, EDIT);
		printf("%s%ld %d is %s\n%s", color, gettimeofday_milisecond(), thread->order, message, RESET);
		status = SUCCESS;
	}
	else
		status = FAIL;
	pthread_mutex_unlock(thread->mutex[TO_PUT]);
	return (status);
}

char	philo_eat(t_thread_data *philo)
{
	access_time_last_eat(philo, EDIT);
	if (put_status(philo, GREEN, EAT, 0) == SUCCESS)
	{
		usleep(philo->time[TO_EAT] * 1000);
		return (SUCCESS);
	}
	else
		return (FAIL);
}

char	philo_sleep(t_thread_data *philo)
{
	if (put_status(philo, CYAN, SLEEP, 0) == SUCCESS)
	{
		usleep(philo->time[TO_SLEEP] * 1000);
		return (SUCCESS);
	}
	else
		return (FAIL);
}

char	philo_think(t_thread_data *philo)
{
	if (put_status(philo, MAGENTA, THINK, 0) == SUCCESS)
		return (SUCCESS);
	else
		return (FAIL);
}

void	*monitor_action(void *data)
{
	t_thread_data	*monitor;
	long			time_now;
	long			time_last_eat;

	monitor = (t_thread_data *)data;
	while (1)
	{
		time_last_eat = access_time_last_eat(monitor, READ);
		time_now = gettimeofday_milisecond();
		if (time_now - time_last_eat > monitor->time[TO_DIE])
		{
			put_status(monitor, RED, DIE, 1);
			break ;
		}
		usleep(10000);
	}
	return (data);
}

void	*philo_action(void *data)
{
	t_thread_data	*philo;
	t_thread_data	*minimoni;
	char			status;

	philo = (t_thread_data *)data;
	minimoni = philo->monitor;
	pthread_create(&(minimoni->thread_id), NULL, &monitor_action, minimoni);
	if (philo->order % 2 == 1)
		usleep(200);
	status = SUCCESS;
	while (status == SUCCESS)
	{
		pthread_mutex_lock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_lock(philo->mutex[LEFT_FORK]);
		status = philo_eat(philo);
		pthread_mutex_unlock(philo->mutex[RIGHT_FORK]);
		pthread_mutex_unlock(philo->mutex[LEFT_FORK]);
		status =  philo_sleep(philo);
		status = philo_think(philo);
	}
	pthread_join(minimoni->thread_id, NULL);
	return (data);
}

char	run_thread(t_manage_data *mdata)
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
	pthread_mutex_destroy(&(mdata->death));
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
	char			return_status;

	if (!(5 <= argc && argc <= 6))
		return (put_arg_error("number of argument"));
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