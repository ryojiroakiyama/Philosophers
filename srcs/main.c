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

char	set_state_time(char **argv, t_state *state)
{
	bool	nonnum_check;

	state->number_of_philosophers = ft_atoi(argv[1], &nonnum_check);
	if (state->number_of_philosophers < 0 || nonnum_check)
		return (1);
	state->time_to_die = ft_atol(argv[2], &nonnum_check);
	if (state->time_to_die < 0 || nonnum_check)
		return (1);
	state->time_to_eat = ft_atol(argv[3], &nonnum_check);
	if (state->time_to_eat < 0 || nonnum_check)
		return (1);
	state->time_to_sleep = ft_atol(argv[4], &nonnum_check);
	if (state->time_to_sleep < 0 || nonnum_check)
		return (1);
	if (argv[5])
	{
		state->time_tobe_satisfied = ft_atol(argv[5], &nonnum_check);
		if (state->time_tobe_satisfied < 0 || nonnum_check)
			return (1);
	}
	else
		state->time_tobe_satisfied = -1;
	return (0);
}

char	put_invalid_arg_error(void)
{
	printf("invalid argument format\n");
	printf("type and order of arguments required:\n");
	printf("1: number_of_philosophers\n");
	printf("2: time_to_die\n");
	printf("3: time_to_eat\n");
	printf("4: time_to_sleep\n");
	printf("5: number_of_times_each_philosopher_must_eat (is optional)\n");
	return (1);
}

void	put_state(t_state *state)
{
	printf("state:\n");
	printf("time program start     %ld\n", state->time_last_eat);
	printf("number_of_philosophers %d\n", state->number_of_philosophers);
	printf("time_to_die            %ld\n", state->time_to_die);
	printf("time_to_eat            %ld\n", state->time_to_eat);
	printf("time_to_sleep          %ld\n", state->time_to_sleep);
	printf("time_tobe_satisfied    %ld\n", state->time_tobe_satisfied);
}

char	put_error(char *message)
{
	printf("%s\n", message);
	return (1);
}

char	set_state(int argc, char **argv, t_state *state)
{
	struct timeval 	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (put_error("gettimeofday"));
	state->time_last_eat = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (!(5 <= argc && argc <= 6) || set_state_time(argv, state))
		return (put_invalid_arg_error());
	state->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * state->number_of_philosophers);
	if (!state->forks)
		return (put_error("malloc"));
	state->threads = (pthread_t *)malloc(sizeof(pthread_t) * state->number_of_philosophers);
	if (!state->threads)
	{
		free(state->forks);
		return (put_error("malloc"));
	}
	return (0);
}

void	*philosopher(void *state)
{
	state = (t_state *)state;
//	pthread_mutex_lock(&mutex);
	put_state(state);
//	pthread_mutex_unlock(&mutex);
	return (0);
}

char	run_thread(t_state *state)//どうやってそれぞれのスレッド（フィロソファ)に何番目なのか知らせる、構造体の変数使ったら他も共有してるから、、
{
	int	thread_cnt;

	thread_cnt = 0;
	while (thread_cnt < state->number_of_philosophers)
	{
		pthread_mutex_init(state->forks + thread_cnt, NULL);
		pthread_create(state->threads + thread_cnt, NULL, &philosopher, state);
		thread_cnt++;
	}
	thread_cnt = 0;
	while (thread_cnt < state->number_of_philosophers)
	{
		pthread_join(state->threads[thread_cnt], NULL);
		pthread_mutex_destroy(state->forks + thread_cnt);
		thread_cnt++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (set_state(argc, argv, &state))
		return (1);
	if (run_thread(&state))
		return (1);
	free(state.forks);
	free(state.threads);
	// if (system("leaks a.out >/dev/null"))
	// 	system("leaks a.out");
	return (0);
}