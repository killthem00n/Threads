#include <stdio.h>
#include <pthread.h>

void *return_index(void *args)
{
	int *id = (int *) args;
	printf("I am the %d thread!\n", *id);
	
	return NULL;
}

int main(void)
{
	pthread_t THREADS[5];
	int tmp[5];
	int *status;
	
	for (int i = 0; i < 5; i++){
		tmp[i] = i;
		pthread_create(&THREADS[i], NULL, return_index, (void *)&tmp[i]);
	}
	
	for (int i = 0; i < 5; i++){
		pthread_join(THREADS[i], (void **)&status);
	}
	
	return 0;
}
