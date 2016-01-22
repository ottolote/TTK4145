// gcc 4.7.2 +
// gcc -std=gnu99 -Wall -g -o helloworld_c helloworld_c.c -lpthread

#include <pthread.h>
#include <stdio.h>

int i = 0;

pthread_mutex_t mutex_i;



// Note the return type: void*
void* reduce_i(){
    for (int j = 0; j<1000000; j++) {
        pthread_mutex_lock (&mutex_i);
        i--;
        pthread_mutex_unlock (&mutex_i);
    }
    return NULL;
}


// Note the return type: void*
void* increase_i(){
    for (int j = 0; j<1000000; j++) {
        pthread_mutex_lock (&mutex_i);
        i++;
        pthread_mutex_unlock (&mutex_i);
    }
    return NULL;
}

int main(){
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, reduce_i, NULL);
    pthread_create(&thread2, NULL, increase_i, NULL);
    // Arguments to a thread would be passed here ---------^
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("%d\t",i);
    return 0;
}
