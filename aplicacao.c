#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMITE_VOOS 15

#define RESET   "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE   "\033[32m"
#define AMARELO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CIANO   "\033[36m"
#define BRANCO  "\033[37m"

typedef struct {
    int numero;
    char companhia[20];
    char destino[30];
    int portao;
    char hora[6]; // formato "HH:MM"
    char status[15];
} Voo;

typedef struct nodo {
    Voo voo;
    struct nodo* link;
} nodo;

// Função auxiliar para comparar horários (formato HH:MM)
int compararHora(const char* h1, const char* h2) {
    int hora1, min1, hora2, min2;
    sscanf(h1, "%d:%d", &hora1, &min1);
    sscanf(h2, "%d:%d", &hora2, &min2);
    if (hora1 != hora2) return hora1 - hora2;
    return min1 - min2;
}

// Cor do status
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

// Inserção ordenada por horário
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

    // Verifica se excedeu o limite de 15 voos
    int contador = 0;
    aux = *inicio;
    nodo* anteriorUltimo = NULL;
    while (aux != NULL) {
        contador++;
        if (contador == LIMITE_VOOS) anteriorUltimo = aux;
        aux = aux->link;
    }
    if (contador > LIMITE_VOOS && anteriorUltimo && anteriorUltimo->link) {
        nodo* remover = anteriorUltimo->link;
        printf("\n[Voo removido] O voo das %s para %s saiu da tabela.\n", remover->voo.hora, remover->voo.destino);
        anteriorUltimo->link = NULL;
        free(remover);
    }
}

void exibirPainel(nodo* inicio) {
    printf("\033[H\033[J"); // Limpa terminal (Linux)
    printf("\n%-6s %-6s %-15s %-20s %-6s %-15s\n", 
        "Hora", "Voo", "Companhia", "Destino", "Portao", "Status");
    printf("-------------------------------------------------------------------------------\n");
    nodo* aux = inicio;
    while (aux != NULL) {
        printf("%-6s %-6d %-15s %-20s %-6d %s%-15s%s\n",
            aux->voo.hora, aux->voo.numero, aux->voo.companhia,
            aux->voo.destino, aux->voo.portao, corStatus(aux->voo.status), aux->voo.status, RESET);
        aux = aux->link;
    }
}

void mostrarVoo(nodo* inicio, int numero) {
    exibirPainel(inicio);
    printf("\n---------------------------------------------------------------------------\n");
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
            char novoStatus[15], confirmacao;
            printf("Novo status (Partida, Em voo, Aterrissado, Cancelado, Atrasado, Embarque, Desembarque): ");
            fgets(novoStatus, 15, stdin); strtok(novoStatus, "\n");
            printf("Confirmar alteracao para '%s'? (S/N): ", novoStatus);
            scanf(" %c", &confirmacao); getchar();
            if (confirmacao == 'S' || confirmacao == 's') {
                strcpy(aux->voo.status, novoStatus);
                printf("Status alterado com sucesso.\n");
            } else {
                printf("Alteracao cancelada.\n");
            }
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
    Voo voos[10] = {
        {1000, "ITA", "Manaus", 1, "21:19", "Atrasado"},
        {1001, "AZUL", "Fortaleza", 16, "08:20", "Partida"},
        {1002, "VOEPASS", "Brasilia", 20, "20:19", "Partida"},
        {1003, "VOEPASS", "Belem", 9, "11:36", "Aterrissado"},
        {1004, "AZUL", "Belem", 1, "15:32", "Partida"},
        {1005, "AZUL", "Belem", 14, "04:30", "Aterrissado"},
        {1006, "VOEPASS", "Curitiba", 7, "10:54", "Aterrissado"},
        {1007, "GOL", "Salvador", 12, "11:44", "Aterrissado"},
        {1008, "LATAM", "Belem", 2, "14:32", "Em voo"},
        {1009, "AZUL", "Porto Alegre", 6, "21:20", "Cancelado"}
    };
    for (int i = 0; i < 10; i++) inserirOrdenado(lista, voos[i]);
}

int main() {
    nodo* lista = NULL;
    preencherVoosIniciais(&lista);
    char opcao;
    do {
        exibirPainel(lista);
        printf("\n[A] Adicionar  [I] Info Voo  [S] Alterar Status  [E] Excluir  [X] Sair\n");
        printf("Opcao: ");
        scanf(" %c", &opcao); getchar();

        if (opcao == 'A' || opcao == 'a') {
            Voo novo;
            printf("Numero do voo: "); scanf("%d", &novo.numero); getchar();
            printf("Companhia: "); fgets(novo.companhia, 20, stdin); strtok(novo.companhia, "\n");
            printf("Destino: "); fgets(novo.destino, 30, stdin); strtok(novo.destino, "\n");
            printf("Portao: "); scanf("%d", &novo.portao); getchar();
            printf("Hora (HH:MM): "); fgets(novo.hora, 6, stdin); getchar(); strtok(novo.hora, "\n");
            printf("Status (Partida, Em voo, Aterrissado, Cancelado, Atrasado, Embarque, Desembarque): "); fgets(novo.status, 15, stdin); strtok(novo.status, "\n");
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