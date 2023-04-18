#include <stdio.h> #include <stdlib.h> #include <pthread.h>

#define MAX_CUSTOMERS 10
 
#define MAX_RESOURCES 5


int available[MAX_RESOURCES];
int maximum[MAX_CUSTOMERS][MAX_RESOURCES];
int allocation[MAX_CUSTOMERS][MAX_RESOURCES]; int need[MAX_CUSTOMERS][MAX_RESOURCES];
int num_customers; int num_resources;

pthread_mutex_t mutex;


void *customer_thread(void *arg) { int customer_id = *(int *)arg;

// Generate random requests for resources int request[MAX_RESOURCES];
for (int i = 0; i < num_resources; i++) {
request[i] = rand() % (maximum[customer_id][i] + 1);
}


// Request resources pthread_mutex_lock(&mutex);
printf("Customer %d requesting resources: ", customer_id); for (int i = 0; i < num_resources; i++) {
printf("%d ", request[i]);
}
printf("\n");


// Check if request can be granted int safe = 1;
 
for (int i = 0; i < num_resources; i++) {
if (request[i] > need[customer_id][i] || request[i] > available[i]) { safe = 0;
break;
}
}


if (safe) {
// Temporarily allocate resources
for (int i = 0; i < num_resources; i++) { available[i] -= request[i]; allocation[customer_id][i] += request[i]; need[customer_id][i] -= request[i];
}


// Check if system is in safe state
// (Banker's algorithm safety check) int work[MAX_RESOURCES];
int finish[MAX_CUSTOMERS] = {0}; for (int i = 0; i < num_resources; i++) {
work[i] = available[i];
}


int num_finished = 0;
while (num_finished < num_customers) { int found = 0;
for (int i = 0; i < num_customers; i++) { if (!finish[i]) {
int j;
for (j = 0; j < num_resources; j++) {
 
if (need[i][j] > work[j]) { break;
}
}
if (j == num_resources) {
for (int k = 0; k < num_resources; k++) { work[k] += allocation[i][k];
}
finish[i] = 1;
found = 1; num_finished++;
printf("Customer %d safely allocated resources\n", i); break;
}
}
}


if (!found) { break;
}
}


if (num_finished == num_customers) { printf("System is in safe state\n");
} else {
printf("System is in unsafe state\n");


// Rollback allocation
for (int i = 0; i < num_resources; i++) { available[i] += request[i];
 
allocation[customer_id][i] -= request[i]; need[customer_id][i] += request[i];
}
}
} else {
printf("Customer %d request denied\n", customer_id);
}


// Release resources

