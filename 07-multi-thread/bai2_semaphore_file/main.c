// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#define MAX_NUM_THREAD 3
#define MAX_NUM_WRITE 5

sem_t sem_write;
int cnt = 0;
void* thread(void* arg) 
{
	for (int i = 0; i < MAX_NUM_WRITE; i++) {
	    sem_wait(&sem_write);
		cnt++;
		const char* filename = (cnt % 2 == 0) ? "file1.txt" : "file2.txt"; 
		FILE* fp = fopen(filename, "a");
		if (fp) {
            fprintf(fp, "Thread %ld wrote cnt = %d\n", pthread_self(), cnt);
            fclose(fp);			
		}
	    sem_post(&sem_write); 
		sleep(1);		
	}
}

int main() 
{
	pthread_t t[MAX_NUM_THREAD];
	sem_init(&sem_write, 0, 1); 

	int i;
	for (i = 0 ; i < MAX_NUM_THREAD ; i ++)
	{
	pthread_create(&t[i],NULL,thread , NULL); 
	}

	for (i = 0 ; i < MAX_NUM_THREAD ; i ++)
	{
	pthread_join(t[i],NULL); 
	}
	
	sem_destroy(&sem_write); 
    printf("\nAll threads finished. Final cnt = %d\n", cnt);
	return 0;
}
