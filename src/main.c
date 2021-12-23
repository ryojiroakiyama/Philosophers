#include "philo.h"

typedef enum e_option
{
	NUM_OF_PHILO,
	TIME_TO_DIE,
	TIME_TO_EAT,
	TIME_TO_SLEEP,
	TIMES_PHILO_MUST_EAT,
	OPTION_NUM
}	t_option;

/*
**  argv[1] options[0]: number_of_philosophers, int
**      [2]        [1]: time_to_die, long
**      [3]        [2]: time_to_eat, long
**      [4]        [3]: time_to_sleep, long
**      [5]        [4]: number_of_times_each_philosopher_must_eat, int
*/
static t_status get_options(int argc, char *argv[], long options[OPTION_NUM])
{
	bool	invalid;
	int		idx_arg;
	int		idx_options;

	options[TIMES_PHILO_MUST_EAT] = UNSPECIFIED;
	idx_arg = 1;
	idx_options = 0;
	while (idx_arg < argc)
	{
		if (idx_options % 4)
			options[idx_options] = ft_atol(argv[idx_arg], &invalid);
		else
			options[idx_options] = ft_atoi(argv[idx_arg], &invalid);
		if (options[idx_options] < 0 || invalid)
			return (put_arg_error(argv[idx_arg]));
		idx_arg++;
		idx_options++;
	}
	return(SUCCESS);
}

static t_status	set_mdata_number(t_manage_data *mdata, long options[OPTION_NUM])
{
	t_time	idx_time;

	mdata->philo_num = options[NUM_OF_PHILO];
	mdata->times_must_eat = options[TIMES_PHILO_MUST_EAT];
	idx_time = 0;
	while (idx_time < TIME_NUM)
	{
		mdata->time[idx_time] = 0;
		if (idx_time <= TO_SLEEP)
			mdata->time[idx_time] = options[idx_time + 1];
		idx_time++;
	}
	mdata->life_flag = NO_ONE_DIED;
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_manage_data	mdata;
	t_status		return_status;
	long			options[OPTION_NUM];

	if (!(argc == 5 || argc == 6))
		return (put_arg_error("number of arguments"));
	mdata = (t_manage_data){0};
	if (get_options(argc, argv, options) == SUCCESS && \
		set_mdata_number(&mdata, options) == SUCCESS && \
		set_mdata_memory(&mdata) == SUCCESS && \
		set_thread_data(&mdata) == SUCCESS && \
		run_thread(&mdata) == SUCCESS)
			return_status = SUCCESS;
	else
		return_status = FAIL;
	free_mdata_memory(&mdata);
	return (return_status);
}
