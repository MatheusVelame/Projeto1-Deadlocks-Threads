#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumidor = PTHREAD_COND_INITIALIZER;

int buffer[5];
int cont = 0;

void produtor(void* arg){

    int produto;

    while(1){

        produto++;

        pthread_mutex_lock(&mutex);

        while(cont == 5){
            printf("Buffer cheio. Produtor esperando!\n");
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        buffer[cont] = produto;
        cont++;
        printf("Produtor adicionou um item ao buffer! Produtos no buffer: %d\n", cont);

        pthread_cond_signal(&cond_consumidor);
        pthread_mutex_unlock(&mutex);

    }

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