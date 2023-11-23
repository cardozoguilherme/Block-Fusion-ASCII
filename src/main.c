/**
 * main.h
 * Created on Nov, 19th 2023
 * Authors: Cláudio Alves, Guilherme Cardozo and Leonardo Cardoso
 * For: PIF 23.2 class
 */

#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RIGHT 100
#define LEFT 97
#define DOWN 115
#define SPACE 32

int x = 12, y = 2;
int incX = 12, incY = 2;
int bloco = 0;

struct Player
{
    int pontos;
    char nome[30];
};

typedef struct
{
    int x; // Só tem um X
    int y; // Só tem um Y
    char type;
} Block;

struct lista
{
    Block block;
    struct lista *next;
};

Block findBlock(int index, struct lista *head)
{
    Block block;

    struct lista *n = head;

    block.type;
    block.x = 0;
    block.y = 0;

    int cont = 0;

    if (index == -1)
    {
        return block;
    }

    while (n != NULL)
    {
        cont++;
        if (cont == index)
        {
            // Passando os atributos do item achado para a instância local
            block.x = n->block.x;
            block.y = n->block.y;
            block.type = n->block.type;

            break;
        }
        n = n->next;
    }

    return block;
}

int findBlockByXY(int xLocal, int yLocal, struct lista *head)
{
    struct lista *n = head;

    int index_block = 0;

    if (n == NULL)
    {
        return -1;
    }

    while (n != NULL)
    {
        if (n->block.x == xLocal && n->block.y == yLocal)
        {
            return index_block;
        }

        index_block++;
        n = n->next;
    }

    return -1;
}

int lenListBlock(struct lista *head)
{
    struct lista *n = head;

    int len = 0;

    while (n != NULL)
    {
        len++;
        n = n->next;
    }

    return len;
}

// Adicionar bloco na lista
void addBlock(struct lista **head, int x, int y, char type)
{
    struct lista *n = *head;

    // Lista temporária - APENAS RECEBER OS VALORES DO NOVO BLOCO A SER ADICIONADO
    struct lista *novo = (struct lista *)malloc(sizeof(struct lista));

    // Adicioando os atributos do novo bloco
    novo->block.x = x;
    novo->block.y = y;
    novo->block.type = type;
    novo->next = NULL;

    // Verificação se a lista é nula, caso sim, ele irá transformar
    // o novo item no cabeçalho da lista.
    if (*head == NULL)
    {
        *head = novo;
    }
    else
    {
        // Percorrendo a lista inteira até chegar no final dela
        // Assim, adicionando o item ao final da lista
        while (n->next != NULL)
        {
            n = n->next;
        }
        n->next = novo;
    }
}

// Checar colisão do próximo bloco em baixo
// Se tiver um bloco abaixo, ele retorna True
// Se não tiver um bloco abaixo, ele retorna False
int checkCollisionDown(struct lista *head, int x, int y)
{
    struct lista *n = head;
    while (n != NULL)
    {
        if (n->block.y == y + 1 && n->block.x == x)
        {
            return 1;
        }
        n = n->next;
    }
    return 0;
}

int checkCollisionLeft(struct lista *head, int x, int y)
{
    struct lista *n = head;
    while (n != NULL)
    {
        if (n->block.y == y && n->block.x == x - 1)
        {
            return 1;
        }
        n = n->next;
    }
    return 0;
}

int checkCollisionRight(struct lista *head, int x, int y)
{
    struct lista *n = head;
    while (n != NULL)
    {
        if (n->block.y == y && n->block.x == x + 1)
        {
            return 1;
        }
        n = n->next;
    }
    return 0;
}

void printBlockPlaced(struct lista *head, int index)
{
    Block block;
    block = findBlock(index, head);

    if (block.type == '#')
    {
        screenSetColor(RED, DARKGRAY);
    }
    else if (block.type == 'X')
    {
        screenSetColor(GREEN, DARKGRAY);
    }
    else
    {
        screenSetColor(CYAN, DARKGRAY);
    }
    screenGotoxy(block.x, block.y);
    printf("%c", block.type);
}

void printBlockMovement(int xNext, int yNext, char type)
{
    if (type == '#')
    {
        screenSetColor(RED, DARKGRAY);
    }
    else if (type == 'X')
    {
        screenSetColor(GREEN, DARKGRAY);
    }
    else
    {
        screenSetColor(CYAN, DARKGRAY);
    }

    screenGotoxy(x, y);
    printf(" ");
    x = xNext;
    y = yNext;
    screenGotoxy(x, y);
    printf("%c", type);
}

void removeBlock(struct lista **head, int index)
{
    // Se a lista estiver vazia
    if (*head == NULL)
        return;

    struct lista *temp = *head;

    // Se o índice for 0 (remover a cabeça)
    if (index == 0)
    {
        *head = temp->next; // Mude a cabeça
        free(temp);         // Libere a antiga cabeça
        return;
    }

    // Encontre o nó anterior ao nó a ser deletado
    for (int i = 0; temp != NULL && i < index - 1; i++)
        temp = temp->next;

    // Se o índice for mais do que o número de nós
    if (temp == NULL || temp->next == NULL)
        return;

    // Nó temp->next é o nó a ser deletado
    // Armazene o ponteiro para o próximo nó a ser deletado
    struct lista *next = temp->next->next;

    // Desconecte o nó da lista encadeada
    free(temp->next); // Libere memória

    temp->next = next; // Desconecte o nó deletado da lista
}

void printScore(char nome_jogador1[30], int pontos_jogador1, char nome_jogador2[30], int pontos_jogador2, int jogador_da_vez)
{
    screenGotoxy(MAXX, 0);

    if (jogador_da_vez) {
        screenSetColor(YELLOW, DARKGRAY);
        printf("| (JOGADOR %s) Pontos: %d", nome_jogador1, pontos_jogador1);
        screenGotoxy(MAXX, 2);
        screenSetColor(LIGHTMAGENTA, DARKGRAY);
        printf("> (JOGADOR %s) Pontos: %d", nome_jogador2, pontos_jogador2);
    } else {
        screenSetColor(LIGHTMAGENTA, DARKGRAY);
        printf("> (JOGADOR %s) Pontos: %d\n", nome_jogador1, pontos_jogador1);
        screenGotoxy(MAXX, 2);
        screenSetColor(YELLOW, DARKGRAY);
        printf("| (JOGADOR %s) Pontos: %d", nome_jogador2, pontos_jogador2);
    }
}

int main()
{
    static int ch = 0;

    // Coletando o nome do primeiro jogador
    struct Player players[2];

    printf("Nome do primeiro jogador: ");
    scanf("%s", players[0].nome);
    players[0].pontos = 0;

    printf("Nome do segundo jogador: ");
    scanf("%s", players[1].nome);
    players[1].pontos = 0;

    // Inicializa a lista dos blocos (placed)
    struct lista *head = NULL;

    screenInit(1);
    keyboardInit();

    // Milisegundos de atualização do jogo
    timerInit(40);

    screenUpdate();

    int wait = 0;
    int wait_limit = 5;
    int jogador_da_vez = 0;

    // Roda o jogo até o input do enter
    while (ch != 10) // se o input for enter, ele para o jogo
    {   
        
        // Handle user input
        if (keyhit())
        {
            // TECLA DO TECLADO (CÓDIGO)
            ch = readch();

            // Se o usuário apertar o A, ele vai para a esquerda
            if (ch == LEFT && incX != 2 && checkCollisionLeft(head, incX, incY) != 1)
            {
                // Decrementa no X (BLOCO EM ANDAMENTO)
                incX--;
            }
            // Se o usuário apertar o D, ele vai para direita
            else if (ch == RIGHT && incX != MAXX - 2 && checkCollisionRight(head, incX, incY) != 1)
            {
                incX++;
            }
            // Se o usuário apertar o S, ele vai para baixo
            else if (ch == DOWN && incY != MAXY - 1 && checkCollisionDown(head, incX, incY) != 1 && wait_limit == 2)
            {
                incY++;
            }
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            
            if (bloco == 0)
            {
                printBlockMovement(incX, incY, 'X');
            }
            else if (bloco == 1)
            {
                printBlockMovement(incX, incY, '#');
            }
            else
            {
                printBlockMovement(incX, incY, 'O');
            }

            screenUpdate();

            printScore(players[0].nome, players[0].pontos, players[1].nome, players[1].pontos, jogador_da_vez);

            if (incY != MAXY - 1 && checkCollisionDown(head, incX, incY) != 1)
            {
                
                if (wait == wait_limit)
                {
                    wait_limit=2;
                    incY++;
                    wait = 0;
                }
            }
            else
            {
                if (bloco == 0)
                {
                    addBlock(&head, incX, incY, 'X');
                }
                else if (bloco == 1)
                {
                    addBlock(&head, incX, incY, '#');
                }
                else
                {
                    addBlock(&head, incX, incY, 'O');
                }

                bloco = rand() % 4;

                int dest_bloco = 0;
                
                for (int i = 2; i < MAXX - 2; i++)
                {
                    int count = 0;
                    for (int j = 0; j < 4; j++)
                    {
                        if (findBlockByXY(i + j, incY, head) != -1)
                        {
                            count++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (count == 4)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            removeBlock(&head, findBlockByXY(i + j, incY, head));
                            screenGotoxy(i + j, incY);
                            printf(" ");
                            screenUpdate();
                        }
                        players[jogador_da_vez].pontos += 10;
                        dest_bloco = 1;
                    }
                }

                // Ele estoura os blocos verticalmente
                for (int i = 2; i < MAXY - 2; i++)
                {
                    int count = 0;
                    for (int j = 0; j < 4; j++)
                    {
                        if (findBlockByXY(incX, i + j, head) != -1)
                        {
                            count++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (count == 4)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            removeBlock(&head, findBlockByXY(incX, i + j, head));
                            screenGotoxy(incX, i + j);
                            printf(" ");
                            screenUpdate();
                        }
                        players[jogador_da_vez].pontos += 10;
                        dest_bloco = 1;
                    }
                }
                // int indx = findBlockByXY(incX, incY, head);
                // printf("%d", indx);

                incY = 2;
                incX = 12;
                wait = 0;

                if (!dest_bloco) {
                    jogador_da_vez = !jogador_da_vez;
                }
                
                wait_limit=30;
            }

            wait++;

            for (int i = 1; i < lenListBlock(head) + 1; i++)
            {
                printBlockPlaced(head, i);
                screenUpdate();
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
