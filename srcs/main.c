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

// void	*philosopher(void *var)
// {
// 	pthread_mutex_lock(&mutex);
// 	printf("in thread_func:%d\n", *(int *)var);
// 	pthread_mutex_unlock(&mutex);
// 	return (0);
// }

char	set_init_data(char **argv, t_init_data *init_data)
{
	bool	nonnum_check;

	init_data->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (init_data->number_of_philosophers < 0 || nonnum_check)
		return (1);
	init_data->time_to_die = ft_atol(argv[2], &nonnum_check);
	if (init_data->time_to_die < 0 || nonnum_check)
		return (1);
	init_data->time_to_eat = ft_atol(argv[3], &nonnum_check);
	if (init_data->time_to_eat < 0 || nonnum_check)
		return (1);
	init_data->time_to_sleep = ft_atol(argv[4], &nonnum_check);
	if (init_data->time_to_sleep < 0 || nonnum_check)
		return (1);
	if (argv[5])
	{
		init_data->time_tobe_satisfied = ft_atol(argv[5], &nonnum_check);
		if (init_data->time_tobe_satisfied < 0 || nonnum_check)
			return (1);
	}
	else
		init_data->time_tobe_satisfied = -1;
	return (0);
}

char	terminate_by_invalid_arg(void)
{
	printf("invalid argument format\n");
	printf("type and order of arguments required:\n");
	printf("1:number_of_philosophers\n");
	printf("2:time_to_die\n");
	printf("3:time_to_eat\n");
	printf("4:time_to_sleep\n");
	printf("5:number_of_times_each_philosopher_must_eat (is optional)\n");
	return (1);
}

void	put_init_data(t_init_data *init_data)
{
	printf("init_data:\n");
	printf("time_from_start        %ld\n", init_data->time_from_start);
	printf("number_of_philosophers %d\n", init_data->number_of_philosophers);
	printf("time_to_die            %ld\n", init_data->time_to_die);
	printf("time_to_eat            %ld\n", init_data->time_to_eat);
	printf("time_to_sleep          %ld\n", init_data->time_to_sleep);
	printf("time_tobe_satisfied    %ld\n", init_data->time_tobe_satisfied);
}

char	terminate_by_function_error(char *message)
{
	printf("%s\n", message);
	return (1);
}

int	main(int argc, char **argv)
{
	struct timeval 	tv;
	t_init_data		init_data;

	if (gettimeofday(&tv, NULL) == -1)
		return (terminate_by_function_error("gettimeofday"));
	init_data.time_from_start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (!(5 <= argc && argc <= 6) || set_init_data(argv, &init_data))
		return (terminate_by_invalid_arg());
	put_init_data(&init_data);
	// pthread_mutex_init(&mutex, NULL);
	// var = 0;
	// pthread_create(&thre, NULL, &thread_func, &var);
	// pthread_mutex_lock(&mutex);
	// cnt = 50000;
	// while (cnt--)
	// 	var++;
	// pthread_mutex_unlock(&mutex);
	// printf("in main:%d\n", var);
	// pthread_join(thre, NULL);
	// pthread_mutex_destroy(&mutex);
	// system("leaks a.out");
	return (0);
}