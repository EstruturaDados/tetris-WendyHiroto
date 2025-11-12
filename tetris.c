#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da struct para representar uma peça
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Constantes para tamanhos fixos
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Variável global para o próximo ID único
int proximoId = 0;

// Função para gerar uma nova peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    // Tipos possíveis de peças
    char tipos[] = {'I', 'O', 'T', 'L'};
    // Gera um tipo aleatório
    novaPeca.nome = tipos[rand() % 4];
    // Atribui o ID único e incrementa
    novaPeca.id = proximoId++;
    return novaPeca;
}

// Estrutura para a fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;  // Índice da frente da fila
    int tras;    // Índice do final da fila
    int tamanho; // Número atual de elementos
} Fila;

// Função para inicializar a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    // Preenche a fila com peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila->pecas[i] = gerarPeca();
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
        fila->tamanho++;
    }
}

// Função para adicionar uma peça ao final da fila (enqueue)
void enqueue(Fila *fila, Peca peca) {
    if (fila->tamanho < TAMANHO_FILA) {
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
        fila->pecas[fila->tras] = peca;
        fila->tamanho++;
    }
}

// Função para remover uma peça da frente da fila (dequeue)
Peca dequeue(Fila *fila) {
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    return pecaRemovida;
}

// Estrutura para a pilha
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;  // Índice do topo da pilha (-1 se vazia)
} Pilha;

// Função para inicializar a pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Função para verificar se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAMANHO_PILHA - 1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Função para empilhar uma peça (push)
void push(Pilha *pilha, Peca peca) {
    if (!pilhaCheia(pilha)) {
        pilha->topo++;
        pilha->pecas[pilha->topo] = peca;
    }
}

// Função para desempilhar uma peça (pop)
Peca pop(Pilha *pilha) {
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return pecaRemovida;
}

// Função para exibir o estado atual da fila e pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\nEstado atual:\n");
    printf("Fila de pecas\t");
    int i = fila->frente;
    for (int count = 0; count < fila->tamanho; count++) {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % TAMANHO_FILA;
    }
    printf("\nPilha de reserva\t(Topo -> Base): ");
    for (int j = pilha->topo; j >= 0; j--) {
        printf("[%c %d] ", pilha->pecas[j].nome, pilha->pecas[j].id);
    }
    printf("\n\n");
}

// Função principal
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Declara e inicializa fila e pilha
    Fila fila;
    inicializarFila(&fila);
    Pilha pilha;
    inicializarPilha(&pilha);

    // Exibe o estado inicial
    exibirEstado(&fila, &pilha);

    int opcao;
    do {
        // Exibe o menu de opções
        printf("Opcoes de Acao:\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca\n");
        printf("2\tReservar peca\n");
        printf("3\tUsar peca reservada\n");
        printf("0\tSair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:  // Jogar peça: dequeue da fila
                if (fila.tamanho > 0) {
                    dequeue(&fila);
                    // Adiciona uma nova peça ao final da fila
                    enqueue(&fila, gerarPeca());
                    printf("Peca jogada!\n");
                } else {
                    printf("Erro: Fila vazia!\n");
                }
                break;
            case 2:  // Reservar peça: move da frente da fila para o topo da pilha
                if (fila.tamanho > 0 && !pilhaCheia(&pilha)) {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    // Adiciona uma nova peça ao final da fila
                    enqueue(&fila, gerarPeca());
                    printf("Peca reservada!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Erro: Pilha de reserva cheia!\n");
                } else {
                    printf("Erro: Fila vazia!\n");
                }
                break;
            case 3:  // Usar peça reservada: pop da pilha
                if (!pilhaVazia(&pilha)) {
                    pop(&pilha);
                    printf("Peca reservada usada!\n");
                } else {
                    printf("Erro: Pilha de reserva vazia!\n");
                }
                break;
            case 0:  // Sair
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        // Exibe o estado após cada ação (exceto sair)
        if (opcao != 0) {
            exibirEstado(&fila, &pilha);
        }
    } while (opcao != 0);

    return 0;
}
