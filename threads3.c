#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define SHOPS 5
#define MAX_SHOP_STOCK 10000
#define CUSTOMERS 3
#define MAX_CUSTOMER_DEMAND 100000

pthread_mutex_t shop_lock[SHOPS];
int shop_stock[SHOPS];
int customer_demand[CUSTOMERS];

void *customers(void *args)
{
	int *id = (int *) args;
	int shop_buy_id;
	while (customer_demand[*id] != 0) {
		shop_buy_id = rand() % SHOPS;
		if (pthread_mutex_trylock(&shop_lock[shop_buy_id]) != EBUSY) {
			if (shop_stock[shop_buy_id] < customer_demand[*id]) {
				customer_demand[*id] = customer_demand[*id] - shop_stock[shop_buy_id];
				printf("Customer %d demand: %d\n", *id, customer_demand[*id]);
			}
			else if (shop_stock[shop_buy_id] >= customer_demand[*id]) {
				shop_stock[shop_buy_id] = shop_stock[shop_buy_id] - customer_demand[*id];
				customer_demand[*id] = 0;
				printf("Customer %d demand: %d\n", *id, customer_demand[*id]);
			}
			sleep(1);
			pthread_mutex_unlock(&shop_lock[shop_buy_id]);
		} 
	}
	
	return NULL;
}

void *work(void *args)
{
	int shop_restock_id;
	while (customer_demand[0] != 0 || customer_demand[1] != 0 || customer_demand[2] != 0) {								
		shop_restock_id = rand() % SHOPS;
		pthread_mutex_lock(&shop_lock[shop_restock_id]);
		shop_stock[shop_restock_id] += 10000;
		printf("Shop %d restocked: %d\n", shop_restock_id, shop_stock[shop_restock_id]);
		sleep(1);
		pthread_mutex_unlock(&shop_lock[shop_restock_id]);
	}
	
	return NULL;
}

int main(void)
{
	pthread_t customer[CUSTOMERS];
	pthread_t worker;
	int *status;
	int cumulate[CUSTOMERS];
	srand(time(NULL));
	printf("INITIAL VALUES\n");
	for (int i = 0; i < SHOPS; i++) {
		pthread_mutex_init(&shop_lock[i], NULL);
		shop_stock[i] = rand() % MAX_SHOP_STOCK;
		printf("Shop %d stock: %d\n", i, shop_stock[i]);
	}
	for (int i = 0; i < CUSTOMERS; i++) {
		cumulate[i] = i;
		customer_demand[i] = rand() % MAX_CUSTOMER_DEMAND;
		printf("Customer %d demand: %d\n", cumulate[i], customer_demand[i]);
	}
	printf("\n\n\n");
	for (int i = 0; i < CUSTOMERS; i++) {
		cumulate[i] = i;
		pthread_create(&customer[i], NULL, customers, (void *)&cumulate[i]);
	}
	pthread_create(&worker, NULL, work, NULL);
	for (int i = 0; i < CUSTOMERS; i++) {
		pthread_join(customer[i], (void **)&status);
	}
	pthread_join(worker, (void **)&status);
	for (int i = 0; i < SHOPS; i++) {
		pthread_mutex_destroy(&shop_lock[i]);
	}
	
	return 0;
}
