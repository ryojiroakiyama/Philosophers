#include "lab.h"

int main(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	printf("second:%ld micro second:%d\n",tv.tv_sec, tv.tv_usec);
	printf("mili second:%ld %d\n", tv.tv_sec * 1000, tv.tv_usec / 1000);
	return (0);
}