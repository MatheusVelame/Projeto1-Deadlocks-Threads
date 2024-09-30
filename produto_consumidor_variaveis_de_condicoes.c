#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumidor = PTHREAD_COND_INITIALIZER;

void produtor(void* arg){

}

void consumidor(void* arg){

}

int main(){

    pthread_t t_produtor;
    pthread_t t_consumidor;

    pthread_create(t_produtor, NULL, produtor, NULL);
    pthread_create(t_consumidor, NULL, consumidor, NULL);

    pthread_join(t_produtor, NULL);
    pthread_join(t_consumidor, NULL);

    return 0;
}