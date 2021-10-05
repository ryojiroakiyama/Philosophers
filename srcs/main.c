#include "philo.h"

pthread_mutex_t mutex;

void	*philosopher(void *var)
{
	pthread_mutex_lock(&mutex);
	printf("in thread_func:%d\n", *(int *)var);
	pthread_mutex_unlock(&mutex);
	return (0);
}

char	set_state(char **argv, t_state *state);
{
	if	()
		return (1);
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

int	main(int argc, char **argv)
{
	t_state	state;

	if (argc < 4 && 5 < argc)
		return (terminate_by_invalid_arg());
	pthread_mutex_init(&mutex, NULL);
	var = 0;
	pthread_create(&thre, NULL, &thread_func, &var);
	pthread_mutex_lock(&mutex);
	cnt = 50000;
	while (cnt--)
		var++;
	pthread_mutex_unlock(&mutex);
	printf("in main:%d\n", var);
	pthread_join(thre, NULL);
	pthread_mutex_destroy(&mutex);
	system("leaks a.out");
	return (0);
}
//pthread_detach//スレッドの終了を待たない