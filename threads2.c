#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int a = 0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void *calc(void *args)
{
	for (int i = 0; i < 50000000; i++){
		pthread_mutex_lock(&m1);
		a++;
		pthread_mutex_unlock(&m1);
	}
	
	return NULL;
}

int main(void)
{
	pthread_t THREADS[5];
	int *status;
	
	for (int i = 0; i < 5; i++){
		pthread_create(&THREADS[i], NULL, calc, NULL);
	}
	
	for (int i = 0; i < 5; i++){
		pthread_join(THREADS[i], (void **)&status);
	}
	
	printf("CALCULATED!\n%d\n", a);
	
	return 0;
}
