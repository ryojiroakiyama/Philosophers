
//t_life	access_life_flag(t_thread_data *thread, t_access_mode mode)
//{
//	t_life	result;

//	pthread_mutex_lock(thread->mutex[TO_PUT]);
//	if (mode == EDIT)
//		*(thread->is_died) = true;
//	result = *(thread->is_died);
//	pthread_mutex_unlock(thread->mutex[TO_PUT]);
//	return (result);
//}

//static bool	is_full(t_thread_data *monitor, long pretime_last_eat, \
//												long time_last_eat)
//{
//	if (monitor->times_must_eat != UNSPECIFIED && \
//		pretime_last_eat != time_last_eat)
//	{
//		monitor->times_ate++;
//		if (monitor->times_ate >= monitor->times_must_eat)
//		{
//			put_status(monitor, MAGENTA, FULL, END_FULL);
//			return (true);
//		}
//	}
//	return (false);
//}

//philo_index = 0;
//while (philo_index < mdata->philo_num)
//{
//	if (philo_index % 2 == 1)
//		printf("%s", GREEN);
//	else
//		printf("%s", CYAN);
//	put_thread_data(mdata->philos + philo_index);
//	put_thread_data(mdata->monitors + philo_index);
//	printf("%s", RESET);
//	philo_index++;
//}
//exit(0);