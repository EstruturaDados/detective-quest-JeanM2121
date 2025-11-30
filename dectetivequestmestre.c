#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10  // tabela hash
//ESTRUTURA DAS SALAS 
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

//ESTRUTURA DAS PISTAS 
typedef struct Pista {
    char nome[50];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

//ESTRUTURA DA TABELA HASH
typedef struct HashNode {
    char pista[50];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

//FUNÇÕES DE SALA 
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

//FUNÇÕES DE BST
Pista* inserirPista(Pista* raiz, const char* nome) {
    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->nome, nome);
        nova->esquerda = nova->direita = NULL;
        printf("🔎 Nova pista adicionada: %s\n", nome);
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
        printf("📜 %s\n", raiz->nome);
        mostrarPistasEmOrdem(raiz->direita);
    }
}

//FUNÇÕES DA TABELA HASH
int hashFunc(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = hashFunc(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

void mostrarTabelaHash() {
    printf("\n=== Relação Pistas → Suspeitos ===\n");
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            printf("📎 Pista: %-30s → 🕵️ Suspeito: %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

void suspeitocitado() {
    typedef struct {
        char nome[50];
        int qtd;
    } Contador;

    Contador lista[20];
    int qtd = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual) {
            int encontrado = 0;
            for (int j = 0; j < qtd; j++) {
                if (strcmp(lista[j].nome, atual->suspeito) == 0) {
                    lista[j].qtd++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(lista[qtd].nome, atual->suspeito);
                lista[qtd].qtd = 1;
                qtd++;
            }
            atual = atual->prox;
        }
    }

    if (qtd == 0) {
        printf("\nNenhum suspeito associado.\n");
        return;
    }

    Contador mais = lista[0];
    for (int i = 1; i < qtd; i++) {
        if (lista[i].qtd > mais.qtd)
            mais = lista[i];
    }

    printf("\n🕵️ Suspeito mais citado: %s (%d pistas associadas)\n", mais.nome, mais.qtd);
}

//EXPLORAÇÃO DAS SALAS 
void explorarSalas(Sala* atual, Pista** arvorePistas) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);
        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  (e) Esquerda - %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  (d) Direita - %s\n", atual->direita->nome);
        printf("  (p) Ver pistas coletadas\n");
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
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

        //ADICIONAR PISTAS E SUSPEITOS AUTOMATICAMENTE
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Página rasgada de um diário");
            inserirNaHash("Página rasgada de um diário", "Sr. Blackwood");
        }
        else if (strcmp(atual->nome, "Sala Secreta") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Chave antiga");
            inserirNaHash("Chave antiga", "Sra. Violet");
        }
        else if (strcmp(atual->nome, "Galeria de Arte") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Retrato danificado");
            inserirNaHash("Retrato danificado", "Sr. Blackwood");
        }
        else if (strcmp(atual->nome, "Cozinha") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Luvas sujas de sangue");
            inserirNaHash("Luvas sujas de sangue", "Chef Gordon");
        }
        else if (strcmp(atual->nome, "Jardim dos Fundos") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Pegadas na lama");
            inserirNaHash("Pegadas na lama", "Sra. Violet");
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou ao fim do caminho! (%s é uma sala final)\n", atual->nome);
            break;
        }
    }
}

//FUNÇÃO PRINCIPAL
int main() {
    // Criando mapa da mansão
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    hall->esquerda->esquerda = criarSala("Sala Secreta");
    hall->esquerda->direita = criarSala("Galeria de Arte");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Jardim dos Fundos");

    // Ponteiro para árvore de pistas
    Pista* arvorePistas = NULL;

    printf("=== 🏆 Detective Quest: Suspeitos e Solução ===\n");
    explorarSalas(hall, &arvorePistas);

    printf("\n=== 📜 Pistas Coletadas ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista encontrada.\n");
    else
        mostrarPistasEmOrdem(arvorePistas);

    mostrarTabelaHash();
    suspeitocitado();

    printf("\nFIM DE JOGO!\n");
    return 0;
}
