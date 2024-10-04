#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_leitorS 5
#define NUM_escritorS 2

pthread_rwlock_t rwlock;
int shared_data = 0;

void* leitor(void* id) {
    int leitor_id = *((int*)id);
    while (1) {
        pthread_rwlock_rdlock(&rwlock); // Adquire o lock para leitura
        printf("Leitor %d: valor do dado compartilhado é %d\n", leitor_id, shared_data);
        pthread_rwlock_unlock(&rwlock); // Libera o lock
        sleep(rand() % 3 + 1); // Simula tempo de processamento
    }
    return NULL;
}

void* escritor(void* id) {
    int escritor_id = *((int*)id);
    while (1) {
        pthread_rwlock_wrlock(&rwlock); // Adquire o lock para escrita (prioridade sobre leitores)
        shared_data++;
        printf("Escritor %d: atualizou o valor do dado compartilhado para %d\n", escritor_id, shared_data);
        pthread_rwlock_unlock(&rwlock); // Libera o lock
        sleep(rand() % 3 + 1); // Simula tempo de processamento
    }
    return NULL;
}

int main() {
    pthread_t leitors[NUM_leitorS], escritors[NUM_escritorS];
    int leitor_ids[NUM_leitorS], escritor_ids[NUM_escritorS];

    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < NUM_leitorS; i++) {
        leitor_ids[i] = i + 1;
        pthread_create(&leitors[i], NULL, leitor, &leitor_ids[i]);
    }

    for (int i = 0; i < NUM_escritorS; i++) {
        escritor_ids[i] = i + 1;
        pthread_create(&escritors[i], NULL, escritor, &escritor_ids[i]);
    }

    for (int i = 0; i < NUM_leitorS; i++) {
        pthread_join(leitors[i], NULL);
    }
    for (int i = 0; i < NUM_escritorS; i++) {
        pthread_join(escritors[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock); // Destrói o rwlock

    return 0;
}
