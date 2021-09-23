#include "lab.h"

//mutexがあるとないで、thread_funcの出力内容が変わるので確認
//フィロの実装では、pthread_createの第四引数には色々情報を持った構造体のアドレスを渡すことになりそう

pthread_mutex_t mutex;

void	*thread_func(void *var)
{
	pthread_mutex_lock(&mutex);
	printf("in thread_func:%d\n", *(int *)var);
	pthread_mutex_unlock(&mutex);
	return (0);
}

int	main(void)
{
	int			var;
	int			cnt;
	pthread_t	thre;

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