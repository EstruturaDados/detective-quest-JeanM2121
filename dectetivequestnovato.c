#include <stdio.h>
#include <stdlib.h>

//Estrutura de Dados
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

//Criação do Nó 
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro de alocação memória!\n");
        exit(1);
    }
    snprintf(nova->nome, sizeof(nova->nome), "%s", nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

//Função de Exploração 
void explorarSalas(Sala *atual)
{
  char escolha;

  while (atual != NULL)
  {
    printf("\nVocê está em: %s\n", atual->nome);
    if (atual->esquerda == NULL && atual->direita == NULL)
    {
      printf("Fim. Não existe mais caminhos!\n");
      break;
    }

    printf("Escolha um caminho: (D) Direita,(E) Esquerda, (S) sair: ");
    scanf(" %c", &escolha);

    if (escolha == 'E' && atual->esquerda != NULL)
    {
      atual = atual->esquerda; 
    }
    else if (escolha == 'D' && atual->direita != NULL)
    {
      atual = atual->direita; 
    }
    else if (escolha == 'S')
    {
      printf("Você saiu da exploração.\n");
      break;
    }
    else
    {
      printf("Opção inválida ou caminho inexistente!\n");
    }
  }
}
// Função Principal
int main() {
    // Mapa da mansão (árvore binária fixa)
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Jantar");
    hall->esquerda->esquerda = criarSala("Sala Secreta");
    hall->esquerda->direita = criarSala("Galeria de Arte");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Jardim dos Fundos");

    printf("MAPA DA MANSÃO: DECTETIVE QUEST\n");
    explorarSalas(hall);

    // Liberação simples (sem recursão)
    free(hall->esquerda->esquerda);
    free(hall->esquerda->direita);
    free(hall->direita->esquerda);
    free(hall->direita->direita);
    free(hall->esquerda);
    free(hall->direita);
    free(hall);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
