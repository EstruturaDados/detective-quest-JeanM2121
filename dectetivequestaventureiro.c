#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ESTRUTURA DE SALAS 
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ESTRUTURA DE NÓ DA ÁRVORE DE BUSCA
typedef struct Pista {
    char nome[50];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

//FUNÇÕES DA ÁRVORE BINÁRIA DE SALAS 
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    snprintf(nova->nome, sizeof(nova->nome), "%s", nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

//FUNÇÕES DA ÁRVORE DE BUSCA(PISTAS) 
Pista* inserirPista(Pista* raiz, const char* nome) {
    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->nome, nome);
        nova->esquerda = nova->direita = NULL;
        printf("🔎 Pista adicionada: %s\n", nome);
        return nova;
    }

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->direita = inserirPista(raiz->direita, nome);

    return raiz;
}

void mostrarPistasEmOrdem(Pista* raiz) {
    if (raiz != NULL) {
        mostrarPistasEmOrdem(raiz->esquerda);
        printf("%s\n", raiz->nome);
        mostrarPistasEmOrdem(raiz->direita);
    }
}

// FUNÇÃO DE EXPLORAR
void explorarSalas(Sala* atual, Pista** arvorePistas) {
    char escolha;
while (1) {
        printf("\nVocê está em: %s\n", atual->nome);
        printf("Escolha um caminho:\n");
        if (atual->direita != NULL) printf("  (d) direita - %s\n", atual->direita->nome);
        if (atual->esquerda != NULL) printf("  (e) esquerda - %s\n", atual->esquerda->nome);
        printf("  (p) pistas coletadas\n");
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf("%c", &escolha);

        if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (escolha == 'p') {
            printf("\n=== PISTAS COLETADAS ===\n");
            if (*arvorePistas == NULL)
                printf("Nenhuma pista encontrada ainda.\n");
            else
                mostrarPistasEmOrdem(*arvorePistas);
            continue;
        } 
        else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } 
        else {
            printf("\nCaminho inválido! Escolha outro.\n");
            continue;
        }

        //PISTAS AUTOMÁTICAS
        if (strcmp(atual->nome, "Biblioteca") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Livros bagunçados no chão");
        else if (strcmp(atual->nome, "Sala Secreta") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Pegadas de salto alto");
        else if (strcmp(atual->nome, "Galeria de Arte") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Facas sujas de terra");
        else if (strcmp(atual->nome, "Cozinha") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Chão com sangue fresco");
        else if (strcmp(atual->nome, "Jardim dos Fundos") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Roupas jogadas no chão");

        // FIM DE CAMINHO
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou ao fim do caminho! (%s é uma sala final)\n", atual->nome);
            break;
        }
    }
}

//FUNÇÃO PRINCIPAL(MAIN)
int main() {
    // Criação da árvore de salas
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    hall->esquerda->esquerda = criarSala("Sala Secreta");
    hall->esquerda->direita = criarSala("Galeria de Arte");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Jardim dos Fundos");

    // Ponteiro para árvore de pistas
    Pista* arvorePistas = NULL;

    printf("Organização das Pistas\n");
    explorarSalas(hall, &arvorePistas);

    printf("\nPistas Finais\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi encontrada.\n");
    else
        mostrarPistasEmOrdem(arvorePistas);

    printf("\n FIM DE JOGO!\n");
    
    return 0;

}
