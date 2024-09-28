#include <pthread.h> //Biblioteca para criar e gerenciar threads (produtores e consumidores)
#include <semaphore.h> //Biblioteca para utilizar semáforos para sincronizar o acesso ao buffer compartilhado
#include <stdio.h>
#include <unistd.h>  //Biblioteca para usar a função sleep, que introduz uma pausa nas threads

#define FALSE 0
#define TRUE  1

// Semáforos
sem_t mutex;         //Exclusão mútua no acesso ao buffer
sem_t vazio;         //Controla o número de espaços vazios no buffer
sem_t cheio;         //Controla o número de itens no buffer

// Buffer
#define BUFFERVAZIO 0
#define BUFFERCHEIO 1
int buffer; //Representa o buffer compartilhado
int estado = BUFFERVAZIO; //Indica se o buffer está cheio ou vazio
int itens_a_consumir = 30; //Define o total de itens a serem produzidos/consumidos: 3 produtores * 10 itens cada

void* produtor(void* arg) {
    int id = *(int*) arg;
    int i = 0;
    int item;

    printf("Iniciando Produtor %d ...\n", id);
    while (i < 10) {
        //Loop para produzir item - o produtor produz 10 itens, aguardando 2 segundos entre cada produção 
        item = i + (id * 1000);

        sem_wait(&vazio);       //O produtor espera até ter um espaço vazio no buffer
        sem_wait(&mutex);       //Entra na região crítica - o produtor adquire o controle exclusivo do buffer para evitar que um consumidor o acesse

        //Inserir item
        printf("Produtor %d inserindo item %d.\n", id, item);
        buffer = item;
        estado = BUFFERCHEIO; //O item é colocado no buffer e o estado é alterado para BUFFERCHEIO

        sem_post(&mutex);       //Sai da região crítica - o produtor libera o controle do buffer
        sem_post(&cheio);       //Sinaliza aos consumidores que há um item no buffer

        i++;
        sleep(2); //Pausa de 2 segundos
    }
    printf("Produtor %d finalizado.\n", id);
    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*) arg;
    int item;

    printf("Iniciando Consumidor %d ...\n", id);
    while (itens_a_consumir > 0) { //O consumidor tenta consumir itens até que todos tenham sido processados 
        sem_wait(&cheio);       //O consumidor espera paté que haja um item no buffer
        sem_wait(&mutex);       //Entra na região crítica - o consumidor adquire controle exclusivo do buffer

        //Retira item do buffer
        if (estado == BUFFERCHEIO) {
            item = buffer;
            estado = BUFFERVAZIO; //O item é removido do buffer e o estado é alterado para BUFFERVAZIO
            printf("Consumidor %d consumiu item %d.\n", id, item);
            itens_a_consumir--;
        }

        sem_post(&mutex);       //Sai da região crítica - o consumidor libera o controle do buffer
        sem_post(&vazio);       //Sinaliza que o buffer tem espaço vazio para os produtores colocarem novos itens

        sleep(2); //Pausa de 2 segundos
    }
    printf("Consumidor %d finalizado.\n", id);
    return NULL;
}

int main() {
    //Cria 3 threads produtoras e 2 threads consumidoras
    //Cada thread produtora vai gerar 10 itens, e os consumidores vão consumir até que todos os itens sejam processados
    pthread_t prod1, prod2, prod3;
    pthread_t cons1, cons2;

    int id1 = 1, id2 = 2, id3 = 3;

    printf("\n## Programa Produtor-Consumidor com Semaforos ##\n");

    //Inicializa semáforos
    sem_init(&mutex, 0, 1);  //Inicializa o semáforo mutex com valor 1, indicando que o buffer está liberado para acesso (exclusão mútua)
    sem_init(&vazio, 0, 1);  //Início: há um espaço vazio no buffer - produtores podem produzir
    sem_init(&cheio, 0, 0);  //Início: não há itens no buffer - consumidores não podem consumir

    printf("Iniciando threads: Produtores\n");
    pthread_create(&prod1, NULL, produtor, (void*) &id1);
    pthread_create(&prod2, NULL, produtor, (void*) &id2);
    pthread_create(&prod3, NULL, produtor, (void*) &id3);

    printf("Iniciando threads: Consumidores\n");
    pthread_create(&cons1, NULL, consumidor, (void*) &id1);
    pthread_create(&cons2, NULL, consumidor, (void*) &id2);

    //Espera até que cada uma das threads (produtores e consumidores) finalize sua execução
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(prod3, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    //Os semáforos são destruídos após todas as threads terem finalizado
    sem_destroy(&mutex);
    sem_destroy(&vazio);
    sem_destroy(&cheio);

    printf("Terminado processo Produtor-Consumidor.\n\n");

    return 0;
}
