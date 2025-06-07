#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMITE_VOOS 10

//Definicao das Cores para os Status pelo Codigo ANSI
#define RESET   "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE   "\033[32m"
#define AMARELO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CIANO   "\033[36m"
#define BRANCO  "\033[37m"

//Estrutura: Define como um voo e armazenado:
typedef struct {
    int numero;
    char companhia[20];
    char destino[30];
    int portao;
    char hora[6];
    char status[15];
} Voo;

//Estrutura da Lista Encadeada:
typedef struct nodo {
    Voo voo;
    struct nodo* link;
} nodo;

// Funcao para comparar as Horas 
int compararHora(const char* h1, const char* h2) {
    int hora1, min1, hora2, min2;
    sscanf(h1, "%d:%d", &hora1, &min1);
    sscanf(h2, "%d:%d", &hora2, &min2);
    if (hora1 != hora2) return hora1 - hora2;
    return min1 - min2;
} 

// Funcao para definir a Cor dos Status
const char* corStatus(const char* status) {
    if (strcmp(status, "Partida") == 0) return AMARELO;
    if (strcmp(status, "Em voo") == 0) return CIANO;
    if (strcmp(status, "Aterrissado") == 0) return VERDE;
    if (strcmp(status, "Cancelado") == 0) return VERMELHO;
    if (strcmp(status, "Atrasado") == 0) return MAGENTA;
    if (strcmp(status, "Embarque") == 0) return AZUL;
    if (strcmp(status, "Desembarque") == 0) return BRANCO;
    return RESET;
}

// Insercao ordenada por horario
void inserirOrdenado(nodo** inicio, Voo novoVoo) {
    nodo* novo = (nodo*)malloc(sizeof(nodo));
    novo->voo = novoVoo;
    novo->link = NULL;

    nodo *anterior = NULL, *aux = *inicio;
    while (aux != NULL && compararHora(novoVoo.hora, aux->voo.hora) > 0) {
        anterior = aux;
        aux = aux->link;
    }

    if (anterior == NULL) {
        novo->link = *inicio;
        *inicio = novo;
    } else {
        anterior->link = novo;
        novo->link = aux;
    }

    int contador = 0;
    aux = *inicio;
    while (aux != NULL) {
        contador++;
        aux = aux->link;
    }

    if (contador > LIMITE_VOOS) {
        nodo* remover = *inicio;
        printf("\n[Voo removido] O voo das %s para %s saiu da tabela.\n", remover->voo.hora, remover->voo.destino);
        *inicio = remover->link;
        free(remover);
    }
}

void exibirPainel(nodo* inicio) {
    printf("\033[H\033[J");
    printf("\n|---------------------------------------------------------------------------------|\n");
    printf("| Hora  | Voo   | Companhia       | Destino              | Porta | Status         |\n");
    printf("|-------|-------|-----------------|----------------------|-------|----------------|\n");

    nodo* aux = inicio;
    while (aux != NULL) {
        printf("| %-5s | %-5d | %-15s | %-20s | %-5d | %s%-14s%s |\n",
            aux->voo.hora, aux->voo.numero, aux->voo.companhia,
            aux->voo.destino, aux->voo.portao, corStatus(aux->voo.status), aux->voo.status, RESET);
        aux = aux->link;
    }
    printf("|---------------------------------------------------------------------------------|\n");
}

void mostrarVoo(nodo* inicio, int numero) {
    exibirPainel(inicio);
    printf("\n--------------------------------------------------------------------------\n");
    printf("\nInformacoes do Voo Selecionado:\n");

    nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->voo.numero == numero) {
            printf("\n[Voo %d]\nHora: %s\nCompanhia: %s\nDestino: %s\nPortao: %d\nStatus: %s%s%s\n",
                   aux->voo.numero, aux->voo.hora, aux->voo.companhia,
                   aux->voo.destino, aux->voo.portao, corStatus(aux->voo.status), aux->voo.status, RESET);
            break;
        }
        aux = aux->link;
    }
    if (aux == NULL) printf("\nVoo nao encontrado.\n");

    printf("\nPressione ENTER para voltar ao painel...");
    getchar();
}

void alterarStatus(nodo* inicio, int numero) {
    nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->voo.numero == numero) {
            char opcaoStatus;
            printf("\nSelecione o novo status:\n");
            printf("%s1. Embarque%s\n", AZUL, RESET);
            printf("%s2. Desembarque%s\n", BRANCO, RESET);
            printf("%s3. Em voo%s\n", CIANO, RESET);
            printf("%s4. Partida%s\n", AMARELO, RESET);
            printf("%s5. Aterrissado%s\n", VERDE, RESET);
            printf("%s6. Cancelado%s\n", VERMELHO, RESET);
            printf("%s7. Atrasado%s\n", MAGENTA, RESET);
            printf("Opcao: ");
            scanf(" %c", &opcaoStatus); getchar();

            switch(opcaoStatus) {
                case '1': strcpy(aux->voo.status, "Embarque"); break;
                case '2': strcpy(aux->voo.status, "Desembarque"); break;
                case '3': strcpy(aux->voo.status, "Em voo"); break;
                case '4': strcpy(aux->voo.status, "Partida"); break;
                case '5': strcpy(aux->voo.status, "Aterrissado"); break;
                case '6': strcpy(aux->voo.status, "Cancelado"); break;
                case '7': strcpy(aux->voo.status, "Atrasado"); break;
                default: printf("Opcao invalida!\n"); return;
            }
            printf("Status alterado para: %s%s%s\n", corStatus(aux->voo.status), aux->voo.status, RESET);
            return;
        }
        aux = aux->link;
    }
    printf("Voo nao encontrado.\n");
}

void excluirVoo(nodo** inicio, int numero) {
    nodo *aux = *inicio, *anterior = NULL;
    while (aux != NULL && aux->voo.numero != numero) {
        anterior = aux;
        aux = aux->link;
    }
    if (aux == NULL) {
        printf("Voo nao encontrado.\n");
        return;
    }
    char confirmacao;
    printf("Confirmar exclusao do voo %d? (S/N): ", numero);
    scanf(" %c", &confirmacao); getchar();
    if (confirmacao == 'S' || confirmacao == 's') {
        if (anterior == NULL) *inicio = aux->link;
        else anterior->link = aux->link;
        free(aux);
        printf("Voo excluido com sucesso.\n");
    } else {
        printf("Exclusao cancelada.\n");
    }
}

void preencherVoosIniciais(nodo** lista) {
    Voo voos[7] = {
        {1001, "AZUL", "Fortaleza", 16, "08:20", "Partida"},
        {1003, "VOEPASS", "Belem", 9, "11:36", "Aterrissado"},
        {1004, "AZUL", "Belem", 1, "15:32", "Partida"},
        {1006, "VOEPASS", "Curitiba", 7, "10:54", "Aterrissado"},
        {1007, "GOL", "Salvador", 12, "11:44", "Aterrissado"},
        {1008, "LATAM", "Belem", 2, "14:32", "Em voo"},
        {1009, "AZUL", "Porto Alegre", 6, "21:20", "Cancelado"}
    };
    for (int i = 0; i < 7; i++) inserirOrdenado(lista, voos[i]);
}

int vooExiste(nodo* inicio, int numero) {
    nodo* aux = inicio;
    while (aux != NULL) {
        if (aux->voo.numero == numero) return 1;
        aux = aux->link;
    }
    return 0;
}

int main() {
    nodo* lista = NULL;
    preencherVoosIniciais(&lista);
    char opcao;
    do {
        exibirPainel(lista);
        printf("\n\n[A] Adicionar  [I] Info Voo  [S] Alterar Status  [E] Excluir  [X] Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao); getchar();

        if (opcao == 'A' || opcao == 'a') {
            Voo novo;

            do {
                printf("Numero do voo: ");
                scanf("%d", &novo.numero); getchar();
                if (vooExiste(lista, novo.numero)) {
                    printf("%s[Voo existente]%s Tente outro numero.\n", VERMELHO, RESET);
                }
            } while (vooExiste(lista, novo.numero));

            printf("Companhia: "); fgets(novo.companhia, 20, stdin); strtok(novo.companhia, "\n");
            printf("Destino: "); fgets(novo.destino, 30, stdin); strtok(novo.destino, "\n");
            printf("Portao: "); scanf("%d", &novo.portao); getchar();
            printf("Hora (HH:MM): "); fgets(novo.hora, 6, stdin); getchar(); strtok(novo.hora, "\n");

            printf("\nSelecione o status:\n");
            printf("%s1. Embarque%s\n", AZUL, RESET);
            printf("%s2. Desembarque%s\n", BRANCO, RESET);
            printf("%s3. Em voo%s\n", CIANO, RESET);
            printf("%s4. Partida%s\n", AMARELO, RESET);
            printf("%s5. Aterrissado%s\n", VERDE, RESET);
            printf("%s6. Cancelado%s\n", VERMELHO, RESET);
            printf("%s7. Atrasado%s\n", MAGENTA, RESET);
            printf("Opcao: ");
            char opcaoStatus;
            scanf(" %c", &opcaoStatus); getchar();

            switch(opcaoStatus) {
                case '1': strcpy(novo.status, "Embarque"); break;
                case '2': strcpy(novo.status, "Desembarque"); break;
                case '3': strcpy(novo.status, "Em voo"); break;
                case '4': strcpy(novo.status, "Partida"); break;
                case '5': strcpy(novo.status, "Aterrissado"); break;
                case '6': strcpy(novo.status, "Cancelado"); break;
                case '7': strcpy(novo.status, "Atrasado"); break;
                default: printf("Opcao invalida! Usando 'Embarque' como padrao.\n");
                         strcpy(novo.status, "Embarque");
            }

            inserirOrdenado(&lista, novo);
        }
        else if (opcao == 'I' || opcao == 'i') {
            int num;
            printf("Numero do voo: "); scanf("%d", &num); getchar();
            mostrarVoo(lista, num);
        }
        else if (opcao == 'S' || opcao == 's') {
            int num;
            printf("Numero do voo: "); scanf("%d", &num); getchar();
            alterarStatus(lista, num);
        }
        else if (opcao == 'E' || opcao == 'e') {
            int num;
            printf("Numero do voo: "); scanf("%d", &num); getchar();
            excluirVoo(&lista, num);
        }
    } while (opcao != 'X' && opcao != 'x');

    return 0;
}