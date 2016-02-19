/* Example code demonstrating
 * how two threads depending on
 * shared resources (mutexes)
 * between each other can lead
 * to a deadlock.
 *
 * All code provided is as is 
 * and not completely tested
 *
 * Author: Aadil Rizvi
 * Date: 2/18/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <util/util.h>
#include <sys/time.h>

pthread_t thread1;
pthread_t thread2;
pthread_mutex_t my_mutex1;
pthread_mutex_t my_mutex2;

static unsigned int counter;

void thread1_main(void);
void thread2_main(void);
void counter_oper(int thread_num);

void sig_handler(int signum) {
    if (signum != SIGINT) {
        printf("Received invalid signum = %d in sig_handler()\n", signum);
        ASSERT(signum == SIGINT);
    }

    printf("Received SIGINT. Exiting Application\n");

    pthread_cancel(thread1);
    pthread_cancel(thread2);

    exit(0);
}

int main(void) {
    pthread_attr_t attr;
    int status;
 
    signal(SIGINT, sig_handler);

    counter = 0;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024*1024);
   
    printf("Creating thread1\n");
    status = pthread_create(&thread1, &attr, (void*)&thread1_main, NULL);
    if (status != 0) {
        printf("Failed to create thread1 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    printf("Creating thread2\n");
    status = pthread_create(&thread2, &attr, (void*)&thread2_main, NULL);
    if (status != 0) {
        printf("Failed to create thread2 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sig_handler(SIGINT);
    
    return 0;
}

void thread1_main(void) {
    struct timeval ts;

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 1 acquiring lock on mutex-1\n",
                                                   ts.tv_sec,
                                                 ts.tv_usec);
    pthread_mutex_lock(&my_mutex1);
    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 1 successfully acquired mutex-1 lock\n",
                                                            ts.tv_sec,
                                                          ts.tv_usec);

    sleep(10);
    
    pthread_mutex_unlock(&my_mutex1);
    printf("%ld.%06ld: Thread 1 unlocked mutex-1\n",
                                          ts.tv_sec,
                                        ts.tv_usec);

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 1 acquiring lock on mutex-2\n",
                                                   ts.tv_sec,
                                                 ts.tv_usec);
    pthread_mutex_lock(&my_mutex2);
    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 1 successfully acquired mutex-2 lock\n",
                                                            ts.tv_sec,
                                                          ts.tv_usec);

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 1 Ending\n",
                                ts.tv_sec,
                              ts.tv_usec);
}


void thread2_main(void) {
    struct timeval ts;

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 2 acquiring lock on mutex-2\n",
                                                   ts.tv_sec,
                                                 ts.tv_usec);
    pthread_mutex_lock(&my_mutex2);
    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 2 successfully acquired mutex-2 lock\n",
                                                            ts.tv_sec,
                                                          ts.tv_usec);

    sleep(10);
    
    pthread_mutex_unlock(&my_mutex2);
    printf("%ld.%06ld: Thread 2 unlocked mutex-2\n",
                                          ts.tv_sec,
                                        ts.tv_usec);

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 2 acquiring lock on mutex-1\n",
                                                   ts.tv_sec,
                                                 ts.tv_usec);
    pthread_mutex_lock(&my_mutex1);
    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 2 successfully acquired mutex-1 lock\n",
                                                            ts.tv_sec,
                                                          ts.tv_usec);

    gettimeofday(&ts, NULL);
    printf("%ld.%06ld: Thread 2 Ending\n",
                                ts.tv_sec,
                              ts.tv_usec);
}

