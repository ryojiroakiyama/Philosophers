
//t_life	access_life_flag(t_thread_data *thread, t_access_mode mode)
//{
//	t_life	result;

//	pthread_mutex_lock(thread->mutex[TO_LIFE_FLAG]);
//	if (mode == EDIT)
//		*(thread->life_flag) = SOME_ONE_DIED;
//	result = *(thread->life_flag);
//	pthread_mutex_unlock(thread->mutex[TO_LIFE_FLAG]);
//	return (result);
//}
