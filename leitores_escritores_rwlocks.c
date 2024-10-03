#include <pthread.h> // biblioteca para criar e gerencia threads
#include <stdio.h>

pthread_rwlock_t rwlock;
int recurso_compartilhado = 0;
int escritores_espera = 0;

void* leitor(void* arg){
    while (1) {
        if (escritores_espera > 0) {
            continue; // desistência temporária
        }
        pthread_rwlock_rdlock(&rwlock);  // lock de leitura
        printf("Leitor %ld - leitura do valor %d\n", (long)arg, recurso_compartilhado);
        pthread_rwlock_unlock(&rwlock);  // lock - liberar
        break;
    }
    return NULL;
}

void* escritor(void* arg) {
    escritores_espera++;  // escritor - espera

    pthread_rwlock_wrlock(&rwlock);  // lock - adquirir
    recurso_compartilhado += 1;
    printf("Escritor %ld - atualizou valor para %d\n", (long)arg, recurso_compartilhado);
    pthread_rwlock_unlock(&rwlock);  // lock - liberar

    escritores_espera--;  // escritor - indica término escrita
    return NULL;
}

int main() {
    pthread_t t_leitor1, t_leitor2, t_escritor;

    pthread_rwlock_init(&rwlock, NULL);  // lock - inicializar

    // threads - leitores e escriteras
    pthread_create(&t_leitor1, NULL, leitor, (void*)1);
    pthread_create(&t_escritor, NULL, escritor, (void*)1);
    pthread_create(&t_leitor2, NULL, leitor, (void*)2);

    // threads - aguardar conclusão
    pthread_join(t_leitor1, NULL);
    pthread_join(t_escritor, NULL);
    pthread_join(t_leitor2, NULL);

    pthread_rwlock_destroy(&rwlock);  // lock - destruir

    return 0;
}