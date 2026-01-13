#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "auxiliares.h"
#include "cpu.h"
#include "programas.h"

struct ram
{
    int *mem;
    int tamanho;

    CacheLine *cacheL1;
    CacheLine *cacheL2;
    CacheLine *cacheL3;

    long relogioGlobal; // contador do tempo global

    int hitsL1, hitsL2, hitsL3, missRAM; // contadores de acertos, vao mostrar a quantidade de itens encontrados em cada ram
};

void iniciarCache(CacheLine *cache, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        cache[i].tag = -1;         // inicia com endereco invalido da ram
        cache[i].dado = 0;         // inicia dados como 0
        cache[i].valido = 0;       // todos estao vazios
        cache[i].ultimoAcesso = 0; // o tempo de acesso de todos eh 0
    }
}

void atualizarCacheLRU(CacheLine *cache, int tamCache, int endereco, int valor, long tempoAtual)
{

    for (int i = 0; i < tamCache; i++) // procura o endereço por toda a cash
    {
        if (cache[i].valido && cache[i].tag == endereco) // caso encontre
        {
            cache[i].dado = valor;              // atualiza o valor
            cache[i].ultimoAcesso = tempoAtual; // atualiza o tempo
            return;
        }
    }

    // caso nao encontre na cash
    for (int i = 0; i < tamCache; i++) // procura pela cash toda
    {
        if (!cache[i].valido) // procura um enderco vazio
        {
            // preenche com os dados
            cache[i].tag = endereco;
            cache[i].dado = valor;
            cache[i].valido = 1;
            cache[i].ultimoAcesso = tempoAtual;
            return;
        }
    }

    // caso nao encontre o item e nao tenha mais espaco

    int id_vitima = 0;                       // item que sera sobreescrito
    long menorTempo = cache[0].ultimoAcesso; // guarda o tempo de acesso da primeira posição da cash

    for (int i = 1; i < tamCache; i++) // percorre a cash toda
    {
        if (cache[i].ultimoAcesso < menorTempo) // se encontrar um item que foi acessado a mais tempo
        {
            menorTempo = cache[i].ultimoAcesso; // atualiza o menor tempo
            id_vitima = i;                      // marca como item a ser sobreescrito
        }
    }

    // substitui o item que foi usado a mais tempo

    cache[id_vitima].tag = endereco;
    cache[id_vitima].dado = valor;
    cache[id_vitima].valido = 1;
    cache[id_vitima].ultimoAcesso = tempoAtual;
}

RAM *criarRAM(int tam)
{

    RAM *r = malloc(sizeof(RAM));
    if (r == NULL)
    {
        printf("ERRO: Falha ao alocar estrutura RAM\n");
        exit(1);
    }
    r->mem = malloc(tam * sizeof(int)); // cria memoria com lixo
    if (r->mem == NULL)
    {
        printf("ERRO: Falha ao alocar vetor de memoria\n");
        exit(1);
    }
    r->tamanho = tam;
    r->relogioGlobal = 0; // inicia o tempo

    // alocacao de memoria das cash
    r->cacheL1 = (CacheLine *)malloc(TAM_L1 * sizeof(CacheLine));
    r->cacheL2 = (CacheLine *)malloc(TAM_L2 * sizeof(CacheLine));
    r->cacheL3 = (CacheLine *)malloc(TAM_L3 * sizeof(CacheLine));

    if (!r->cacheL1 || !r->cacheL2 || !r->cacheL3)
    {
        printf("ERRO: Falha ao alocar Caches\n");
        exit(1);
    }

    // inicia todas as "linhas" das cash
    iniciarCache(r->cacheL1, TAM_L1);
    iniciarCache(r->cacheL2, TAM_L2);
    iniciarCache(r->cacheL3, TAM_L3);

    // marcadores de acerto
    r->hitsL1 = 0;
    r->hitsL2 = 0;
    r->hitsL3 = 0;
    r->missRAM = 0;
    return r;
}

void reinicializarRAM(RAM *r, int tam)
{
    if (r->mem != NULL)
    {
        free(r->mem);
    }
    if (r->cacheL1 != NULL)
        free(r->cacheL1);
    if (r->cacheL2 != NULL)
        free(r->cacheL2);
    if (r->cacheL3 != NULL)
        free(r->cacheL3);

    r->mem = calloc(tam, sizeof(int));

    if (r->mem == NULL)
    {
        printf("ERRO FATAL: Falha ao alocar %d ints para a RAM.\n", tam);
        exit(1);
    }
    r->tamanho = tam;
    r->relogioGlobal = 0;

    r->cacheL1 = (CacheLine *)malloc(TAM_L1 * sizeof(CacheLine));
    r->cacheL2 = (CacheLine *)malloc(TAM_L2 * sizeof(CacheLine));
    r->cacheL3 = (CacheLine *)malloc(TAM_L3 * sizeof(CacheLine));

    iniciarCache(r->cacheL1, TAM_L1);
    iniciarCache(r->cacheL2, TAM_L2);
    iniciarCache(r->cacheL3, TAM_L3);
}

RAM *criarRAM_vazia(int tam)
{

    RAM *r = criarRAM(tam);

    for (int i = 0; i < tam; i++) // percorre a ram zerando todos os valores
    {
        r->mem[i] = 0;
    }
    return r;
}

RAM *criarRAM_aleatoria(int tam)
{
    srand(time(NULL));
    RAM *r = criarRAM(tam);
    for (int i = 0; i < tam; i++)
        r->mem[i] = rand() % 10; // cria memoria com endereco aleatorio
    return r;
}

int getDado(RAM *r, int endereco)
{
    if (endereco < 0 || endereco >= r->tamanho) // caso o endereco for invalido
        return 0;

    r->relogioGlobal++; // o tempo global atualiza

    for (int i = 0; i < TAM_L1; i++) // procura em toda a cash 1
    {
        if (r->cacheL1[i].valido && r->cacheL1[i].tag == endereco) // se o endereco nao for vazio e tiver a tag correta
        {
            r->hitsL1++;
            r->cacheL1[i].ultimoAcesso = r->relogioGlobal; // atualiza o tempo do ultimo acesso
            return r->cacheL1[i].dado;
        }
    }

    for (int i = 0; i < TAM_L2; i++) // caso nao encontre na cash 1 procura na 2
    {
        if (r->cacheL2[i].valido && r->cacheL2[i].tag == endereco)
        {
            r->hitsL2++;
            int val = r->cacheL2[i].dado;
            r->cacheL2[i].ultimoAcesso = r->relogioGlobal;

            atualizarCacheLRU(r->cacheL1, TAM_L1, endereco, val, r->relogioGlobal); // atualiza a cash 1
            return val;
        }
    }

    for (int i = 0; i < TAM_L3; i++) // caso nao encontre na cash 2 procura na 3
    {
        if (r->cacheL3[i].valido && r->cacheL3[i].tag == endereco)
        {
            r->hitsL3++;
            int val = r->cacheL3[i].dado;
            r->cacheL3[i].ultimoAcesso = r->relogioGlobal;

            // atualiza as duas outras cash
            atualizarCacheLRU(r->cacheL2, TAM_L2, endereco, val, r->relogioGlobal);
            atualizarCacheLRU(r->cacheL1, TAM_L1, endereco, val, r->relogioGlobal);
            return val;
        }
    }

    // se nao encontrar na cash
    r->missRAM++;
    int val = r->mem[endereco];

    // atualiza as 3 cash
    atualizarCacheLRU(r->cacheL3, TAM_L3, endereco, val, r->relogioGlobal);
    atualizarCacheLRU(r->cacheL2, TAM_L2, endereco, val, r->relogioGlobal);
    atualizarCacheLRU(r->cacheL1, TAM_L1, endereco, val, r->relogioGlobal);

    return val;
}

void setDado(RAM *r, int endereco, int conteudo) // adiciona um dado na ram e nas cash
{
    if (endereco < 0 || endereco >= r->tamanho) // endereco invalido
        return;

    r->relogioGlobal++; // atualiza contador de tempo

    r->mem[endereco] = conteudo; // adiciona na ram

    // atualiza as 3 cash
    atualizarCacheLRU(r->cacheL1, TAM_L1, endereco, conteudo, r->relogioGlobal);
    atualizarCacheLRU(r->cacheL2, TAM_L2, endereco, conteudo, r->relogioGlobal);
    atualizarCacheLRU(r->cacheL3, TAM_L3, endereco, conteudo, r->relogioGlobal);
}

void imprimirRAM(RAM *r)
{
    printf("Hits L1: %d | Hits L2: %d | Hits L3: %d | RAM: %d\n",
           r->hitsL1, r->hitsL2, r->hitsL3, r->missRAM);
}

void imprimirCaches(RAM *r)
{
    printf("\n--- Estado da Cache L1 (Tam: %d | LRU) ---\n", TAM_L1);
    for (int i = 0; i < TAM_L1; i++)
    {
        if (r->cacheL1[i].valido)
            printf("[%d]: End:%d Val:%d Tempo:%ld\n", i, r->cacheL1[i].tag, r->cacheL1[i].dado, r->cacheL1[i].ultimoAcesso);
        else
            printf("[%d]: Vazio\n", i);
    }
    printf("\n--- Estado da Cache L2 (Tam: %d | LRU) ---\n", TAM_L2);
    for (int i = 0; i < TAM_L2; i++)
    {
        if (r->cacheL2[i].valido)
            printf("[%d]: End:%d Val:%d Tempo:%ld\n", i, r->cacheL2[i].tag, r->cacheL2[i].dado, r->cacheL2[i].ultimoAcesso);
        else
            printf("[%d]: Vazio\n", i);
    }
    printf("\n--- Estado da Cache L3 (Tam: %d | LRU) ---\n", TAM_L3);
    for (int i = 0; i < TAM_L3; i++)
    {
        if (r->cacheL3[i].valido)
            printf("[%d]: End:%d Val:%d Tempo:%ld\n", i, r->cacheL3[i].tag, r->cacheL3[i].dado, r->cacheL3[i].ultimoAcesso);
        else
            printf("[%d]: Vazio\n", i);
    }
}

void destroiRAM(RAM *r)
{ // desalloca a memoria da ram e das cash
    free(r->mem);
    free(r->cacheL1);
    free(r->cacheL2);
    free(r->cacheL3);
    free(r);
}