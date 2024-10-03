#include <pthread.h> // Biblioteca para criar e gerencia threads
#include <semaphore.h> // Biblioteca para usar semáforos
#include <stdio.h>
#include <unistd.h> // Para usar a função sleep

#define FALSE 0
#define TRUE 1

sem_t mutex, bloqueio_escrita;
int leitores = 0;
int recurso_compartilhado = 0;

void* leitor (void* arg) {
    int id = *(int*) arg;

    while(TRUE) {
        sem_wait(&mutex);
        leitores++;
        if (leitores == 1) {
            sem_wait(&bloqueio_escrita);
        }
        sem_post(&mutex);

        printf("Leitor %d lendo o valor %d do recurso compartilhado.\n", id, recurso_compartilhado);
        sleep(1);

        sem_wait(&mutex);
        leitores--;
        if (leitores == 0) {
            sem_post(&bloqueio_escrita);
        }
        sem_post(&mutex);

        sleep(2);
    }
    return NULL;
}

void* escritor (void* arg) {
    int id = *(int*) arg;

    while(TRUE) {
        sem_wait(&bloqueio_escrita);
        recurso_compartilhado += 10;
        printf("Escritor %d escrevendo o valor %d no recurso compartilhado.\n", id, recurso_compartilhado);
        sleep(1);
        sem_post(&bloqueio_escrita);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t leitores_thread[3], escritores_thread[2];

    int id_leitores[3] = {1, 2, 3};
    int id_escritores[2] = {1, 2};

    printf("\n## Problema dos Leitores/Escritores com Prioridade para Escritores ##\n");

    sem_init (&mutex, 0, 1);
    sem_init(&bloqueio_escrita, 0, 1);

    for (int i=0; i < 3; i++) {
        pthread_create(&leitores_thread[i], NULL, leitor, (void*) &id_leitores[i]);
    }

    for (int i=0; i < 2; i++) {
        pthread_create(&escritores_thread[i], NULL, escritor, (void*) &id_escritores[i]);
    }

    for (int i=0; i < 3; i++) {
        pthread_join(leitores_thread[i], NULL);
    }

    for (int i=0; i < 2; i++) {
        pthread_join(escritores_thread[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&bloqueio_escrita);

    printf("Terminando processo Leitores/Escritores.\n\n");

    return 0;
}
