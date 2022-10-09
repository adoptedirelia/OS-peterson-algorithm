#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include<time.h>
#include<unistd.h>
#include<semaphore.h>
volatile int accnt0 = 0;
volatile int accnt1 = 0;
volatile int flag[2]={0,0};
volatile int turn = 0;
sem_t mutex;
void *worker(void* arg)
{   
    int nLoop=0;
    int nTemp1;
    int nTemp2;
    int nRandom;
    int id = (int)(uintptr_t)arg;
    int peer = 1 - id;
    do{
        flag[id] = 1;
        turn = peer;
        while (flag[peer] == 1 && turn == peer);
        //sem_wait(&mutex);
        nRandom=rand();
        nTemp1 = accnt0;
        nTemp2 = accnt1;
        accnt0 = nTemp1 + nRandom;
        accnt1 = nTemp2 - nRandom;
        if(accnt0+accnt1!=0)
        {
            printf("ohh\n");
            break;
        }
        nLoop++;
        //sem_post(&mutex);
        flag[id]=0;
    }while(nLoop<1000000);
    printf("%d comes to an end, now sum is %d\n",id,nTemp1+nTemp2);
    //printf("it is %d, but now the sum is not zero, it is %d, and it is %d\n",id,nTemp1+nTemp2,nLoop);
}
int main()
{   pthread_t p0,p1;
    srand(0);
    sem_init(&mutex,0,1);
    time_t begin_t = clock();
    pthread_create(&p0, NULL, worker, (void*)0); 
    pthread_create(&p1, NULL, worker, (void*)1);
    pthread_join(p0, NULL);
    pthread_join(p1, NULL);
    time_t end_t = clock();
    double altime = (double)(end_t-begin_t)/CLOCKS_PER_SEC;
    printf("time costs %f\n",altime);
    return 0;
}