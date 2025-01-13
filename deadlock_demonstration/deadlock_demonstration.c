#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

/* Shared structure for all examples */
typedef struct {
    pthread_mutex_t lock1;
    pthread_mutex_t lock2;
} LockPair;

void init_locks(LockPair* locks) {
    pthread_mutex_init(&locks->lock1, NULL);
    pthread_mutex_init(&locks->lock2, NULL);
}

void destroy_locks(LockPair* locks) {
    pthread_mutex_destroy(&locks->lock1);
    pthread_mutex_destroy(&locks->lock2);
}

/* 1. Deadlock Example */
void* deadlock_task1(void* arg) {
    LockPair* locks = (LockPair*)arg;
    
    printf("Task1: Trying to acquire lock1\n");
    pthread_mutex_lock(&locks->lock1);
    printf("Task1: Acquired lock1\n");
    sleep(2);
    
    printf("Task1: Trying to acquire lock2\n");
    pthread_mutex_lock(&locks->lock2);
    printf("Task1: Acquired lock2\n");
    
    pthread_mutex_unlock(&locks->lock2);
    pthread_mutex_unlock(&locks->lock1);
    return NULL;
}

void* deadlock_task2(void* arg) {
    LockPair* locks = (LockPair*)arg;
    
    printf("Task2: Trying to acquire lock2\n");
    pthread_mutex_lock(&locks->lock2);
    printf("Task2: Acquired lock2\n");
    sleep(2);
    
    printf("Task2: Trying to acquire lock1\n");
    pthread_mutex_lock(&locks->lock1);
    printf("Task2: Acquired lock1\n");
    
    pthread_mutex_unlock(&locks->lock1);
    pthread_mutex_unlock(&locks->lock2);
    return NULL;
}

/* 2. Lock Ordering Solution */
void* ordered_task1(void* arg) {
    LockPair* locks = (LockPair*)arg;
    
    printf("OrderedTask1: Acquiring locks in order (lock1 then lock2)\n");
    pthread_mutex_lock(&locks->lock1);
    printf("OrderedTask1: Acquired lock1\n");
    sleep(1);
    
    pthread_mutex_lock(&locks->lock2);
    printf("OrderedTask1: Acquired lock2\n");
    
    printf("OrderedTask1: In critical section\n");
    sleep(1);
    
    pthread_mutex_unlock(&locks->lock2);
    pthread_mutex_unlock(&locks->lock1);
    printf("OrderedTask1: Released all locks\n");
    return NULL;
}

void* ordered_task2(void* arg) {
    LockPair* locks = (LockPair*)arg;
    
    printf("OrderedTask2: Acquiring locks in order (lock1 then lock2)\n");
    pthread_mutex_lock(&locks->lock1);
    printf("OrderedTask2: Acquired lock1\n");
    sleep(1);
    
    pthread_mutex_lock(&locks->lock2);
    printf("OrderedTask2: Acquired lock2\n");
    
    printf("OrderedTask2: In critical section\n");
    sleep(1);
    
    pthread_mutex_unlock(&locks->lock2);
    pthread_mutex_unlock(&locks->lock1);
    printf("OrderedTask2: Released all locks\n");
    return NULL;
}

/* 3. Trylock Solution */
void* trylock_task(void* arg) {
    LockPair* locks = (LockPair*)arg;
    int max_retries = 5;
    int retry_count = 0;
    
    while (retry_count < max_retries) {
        printf("TryLock Task: Attempt %d - Trying lock1\n", retry_count + 1);
        if (pthread_mutex_trylock(&locks->lock1) == 0) {
            printf("TryLock Task: Got lock1\n");
            
            if (pthread_mutex_trylock(&locks->lock2) == 0) {
                printf("TryLock Task: Got lock2 - Executing critical section\n");
                sleep(1);
                pthread_mutex_unlock(&locks->lock2);
                pthread_mutex_unlock(&locks->lock1);
                printf("TryLock Task: Released all locks\n");
                return NULL;
            }
            
            printf("TryLock Task: Couldn't get lock2, releasing lock1\n");
            pthread_mutex_unlock(&locks->lock1);
        }
        
        retry_count++;
        printf("TryLock Task: Backing off and retrying...\n");
        sleep(1);
    }
    
    printf("TryLock Task: Max retries reached\n");
    return NULL;
}

void demonstrate_deadlock() {
    printf("\n=== Demonstrating Deadlock Problem ===\n");
    LockPair locks;
    init_locks(&locks);
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, deadlock_task1, &locks);
    pthread_create(&thread2, NULL, deadlock_task2, &locks);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    destroy_locks(&locks);
}

void demonstrate_lock_ordering() {
    printf("\n=== Demonstrating Lock Ordering Solution ===\n");
    LockPair locks;
    init_locks(&locks);
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, ordered_task1, &locks);
    pthread_create(&thread2, NULL, ordered_task2, &locks);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    destroy_locks(&locks);
}

void demonstrate_trylock() {
    printf("\n=== Demonstrating Trylock Solution ===\n");
    LockPair locks;
    init_locks(&locks);
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, trylock_task, &locks);
    pthread_create(&thread2, NULL, trylock_task, &locks);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    destroy_locks(&locks);
}

int main() {
    int choice;
    
    do {
        printf("\nDeadlock Solutions Demo\n");
        printf("1. Demonstrate Deadlock Problem\n");
        printf("2. Demonstrate Lock Ordering Solution\n");
        printf("3. Demonstrate Trylock Solution\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                demonstrate_deadlock();
                break;
            case 2:
                demonstrate_lock_ordering();
                break;
            case 3:
                demonstrate_trylock();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while(choice != 0);
    
    return 0;
}
