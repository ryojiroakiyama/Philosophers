#include "philo.h"

void	put_manage_data(t_manage_data *mdata)
{
	printf("manage_data:\n");
	printf("philo_num              %d\n", mdata->philo_num);
	printf("times_must_eat         %d\n", mdata->times_must_eat);
	printf("time[TO_DIE]           %ld\n", mdata->time[TO_DIE]);
	printf("time[TO_EAT]           %ld\n", mdata->time[TO_EAT]);
	printf("time[TO_SLEEP]         %ld\n", mdata->time[TO_SLEEP]);
}

void	put_thread_data(t_thread_data *pdata)
{
	printf("philo_data:\n");
	printf("order            %d\n", pdata->order);
	printf("times_must_eat   %d\n", pdata->times_must_eat);
	printf("time[TO_DIE]     %ld\n", pdata->time[TO_DIE]);
	printf("time[TO_EAT]     %ld\n", pdata->time[TO_EAT]);
	printf("time[TO_SLEEP]   %ld\n", pdata->time[TO_SLEEP]);
	printf("time[LAST_EAT    %ld\n", pdata->time[LAST_EAT]);
	printf("time[SUM_EAT]    %ld\n", pdata->time[SUM_EAT]);
	printf("time_last_eat    %ld\n", *(pdata->time_last_eat));
	printf("life_flag        %d\n", *(pdata->life_flag));
}

t_status	put_error(char *message)
{
	printf("error: %s\n", message);
	return (FAIL);
}

t_status	put_arg_error(char *message)
{
	printf("error: %s\n", message);
	printf("type and order of arguments required:\n");
	printf("1: philo_num\n");
	printf("2: time_to_die\n");
	printf("3: time_to_eat\n");
	printf("4: time_to_sleep\n");
	printf("5: number_of_times_each_philosopher_must_eat (is optional)\n");
	return (FAIL);
}
