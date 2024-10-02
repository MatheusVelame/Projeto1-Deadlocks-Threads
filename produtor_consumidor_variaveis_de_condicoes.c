#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumidor = PTHREAD_COND_INITIALIZER;

int buffer[5];
int cont = 0;

void produtor(void* arg){

    int produto = 0;

    while(1){

        produto++;

        pthread_mutex_lock(&mutex);

        while(cont == 5){
            printf("Buffer cheio. Produtor esperando!\n");
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        buffer[cont] = produto;
        cont++;
        printf("Produtor adicionou o item %d ao buffer! Produtos no buffer: %d\n", buffer[cont-1], cont);
        sleep(2);

        pthread_cond_signal(&cond_consumidor);
        pthread_mutex_unlock(&mutex);

    }

}

void consumidor(void* arg){

    while(1){

        pthread_mutex_lock(&mutex);

        while(cont == 0){
            printf("Buffer vazio! Esperando o produtor\n");
            pthread_cond_wait(&cond_consumidor, &mutex);
        }

        int item_consumido = buffer[cont-1];
        cont--;
        printf("Consumidor consumiu o item %d do Buffer! produtos do buffer %d\n", item_consumido, cont);
        sleep(2);

        pthread_cond_signal(&cond_produtor);
        pthread_mutex_unlock(&mutex);

    }

}

int main(){

    pthread_t t_produtor;
    pthread_t t_consumidor;

    pthread_create(&t_produtor, NULL, produtor, NULL);
    pthread_create(&t_consumidor, NULL, consumidor, NULL);

    pthread_join(t_produtor, NULL);
    pthread_join(t_consumidor, NULL);

    return 0;
}