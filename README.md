# Projeto1-Deadlocks-Threads

Esse código implementa soluções para problemas clássicos da programação concorrente utilizando threads com semáforos e variáveis de condições no padrão POSIX. 

## Problema do Produtor/Consumidor

No problema do produtor/consumidor, produtores geram itens e os colocam em um buffer compartilhado,
enquanto consumidores retiram esses itens para processá-los. 
A sincronização entre as threads é fundamental para evitar condições de corrida e acessos simultâneos ao buffer.
Para esse problema, são apresentadas duas soluções diferentes, usando semáforos e váriáveis de ambiente.

## Problema dos Leitores/Escritores
