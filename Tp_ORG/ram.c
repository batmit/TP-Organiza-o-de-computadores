#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "auxiliares.h"
#include "cpu.h"
#include "programas.h"
#include <stdbool.h>

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

// rebaixa a informacao da cache 2 pra 3 e passa da 3 pra ram caso a 3 esteja cheia
void rebaixarParaL3(RAM *r, int endereco, int valor, long tempoOriginal)
{
    for (int i = 0; i < TAM_L3; i++) // percorre toda a cache 3
    {
        if (!r->cacheL3[i].valido) // verifica se esta ocupado
        {
            r->cacheL3[i].tag = endereco;
            r->cacheL3[i].dado = valor;
            r->cacheL3[i].valido = 1;
            r->cacheL3[i].ultimoAcesso = tempoOriginal; // guarda o tempo do ultimo acesso a celula
            return;                                     // retorna apos preencher
        }
    }

    // no caso de a cache 2 estar cheia
    int id_vitima = 0;                            // celula a ser mandada pra cache 3
    long menorTempo = r->cacheL3[0].ultimoAcesso; // pra procuar a que foi acessada a mais tempo

    for (int i = 1; i < TAM_L3; i++) // percorre toda a cache 3
    {
        if (r->cacheL3[i].ultimoAcesso < menorTempo)
        {
            menorTempo = r->cacheL3[i].ultimoAcesso;
            id_vitima = i; // a vitima passa a ser a memoria acessada a mais tempo
        }
    }

    // Simula o funcionamento de um coprocessador que
    //  manda valores da cacheL3 para a RAM
    simularBuffer(r, r->cacheL3, id_vitima);
    // coloca o novo dado na celula vazia (id_vitima)
    r->cacheL3[id_vitima].tag = endereco;
    r->cacheL3[id_vitima].dado = valor;
    r->cacheL3[id_vitima].valido = 1;
    r->cacheL3[id_vitima].ultimoAcesso = tempoOriginal;
}

void rebaixarParaL2(RAM *r, int endereco, int valor, long tempoOriginal) // passa da cache 1 pra 2
{
    for (int i = 0; i < TAM_L2; i++) // verifica se a 2 esta cheia
    {
        if (!r->cacheL2[i].valido) // se encontrar espaço preenche e retorna
        {
            r->cacheL2[i].tag = endereco;
            r->cacheL2[i].dado = valor;
            r->cacheL2[i].valido = 1;
            r->cacheL2[i].ultimoAcesso = tempoOriginal;
            return;
        }
    }

    // se estiver cheia procura a memoria(lru) e passa pra cache 3
    int id_vitima = 0;
    long menorTempo = r->cacheL2[0].ultimoAcesso;

    for (int i = 1; i < TAM_L2; i++) // procura a vitima
    {
        if (r->cacheL2[i].ultimoAcesso < menorTempo)
        {
            menorTempo = r->cacheL2[i].ultimoAcesso;
            id_vitima = i;
        }
    }

    // move pra cache 3
    rebaixarParaL3(r, r->cacheL2[id_vitima].tag, r->cacheL2[id_vitima].dado, r->cacheL2[id_vitima].ultimoAcesso);

    // Ocupa a vaga na cache 2
    r->cacheL2[id_vitima].tag = endereco;
    r->cacheL2[id_vitima].dado = valor;
    r->cacheL2[id_vitima].valido = 1;
    r->cacheL2[id_vitima].ultimoAcesso = tempoOriginal;
}

void promoverParaL1(RAM *r, int endereco, int valor)
{
    r->relogioGlobal++; // atualiza o contador de tempo

    // verifica se a cache 1 esta cheia
    for (int i = 0; i < TAM_L1; i++)
    {
        if (!r->cacheL1[i].valido) // se tiver espaco preenche
        {
            r->cacheL1[i].tag = endereco;
            r->cacheL1[i].dado = valor;
            r->cacheL1[i].valido = 1;
            r->cacheL1[i].ultimoAcesso = r->relogioGlobal;
            for (int j = 1; j < TAM_L2; j++)
            {
                if (r->cacheL2[j].dado == valor)
                {
                    r->cacheL2[j].tag = 0;
                    r->cacheL2[j].dado = 0;
                    r->cacheL2[j].valido = 0;
                    r->cacheL2[j].ultimoAcesso = 0;
                }
            }
            return;
        }
    }

    // se estiver cheia procura uma vitima
    int id_vitima = 0;
    long menorTempo = r->cacheL1[0].ultimoAcesso;

    for (int i = 1; i < TAM_L1; i++) // lru
    {
        if (r->cacheL1[i].ultimoAcesso < menorTempo)
        {
            menorTempo = r->cacheL1[i].ultimoAcesso;
            id_vitima = i;
        }
    }

    // move pra L2
    for (int i = 1; i < TAM_L2; i++) // encontra chave
    {
        if (r->cacheL2[i].dado == valor)
        {
            r->cacheL2[i].tag = r->cacheL1[id_vitima].tag;
            r->cacheL2[i].dado = r->cacheL1[id_vitima].dado;
            r->cacheL2[i].valido = r->cacheL1[id_vitima].valido;
            r->cacheL2[i].ultimoAcesso = r->cacheL1[id_vitima].ultimoAcesso;
        }
    }

    // preenche a cache 1
    r->cacheL1[id_vitima].tag = endereco;
    r->cacheL1[id_vitima].dado = valor;
    r->cacheL1[id_vitima].valido = 1;
    r->cacheL1[id_vitima].ultimoAcesso = r->relogioGlobal;
}

void promoverParaL2(RAM *r, int endereco, int valor)
{
    r->relogioGlobal++; // atualiza o contador de tempo

    // verifica se a cache 2 esta cheia
    for (int i = 0; i < TAM_L2; i++)
    {
        if (!r->cacheL2[i].valido) // se tiver espaco preenche
        {
            r->cacheL2[i].tag = endereco;
            r->cacheL2[i].dado = valor;
            r->cacheL2[i].valido = 1;
            r->cacheL2[i].ultimoAcesso = r->relogioGlobal;
            for (int j = 1; j < TAM_L3; j++) // encontra a (lru)
            {
                if (r->cacheL3[j].dado == valor)
                {
                    r->cacheL3[j].tag = 0;
                    r->cacheL3[j].dado = 0;
                    r->cacheL3[j].valido = 0;
                    r->cacheL3[j].ultimoAcesso = 0;
                }
            }
            return;
        }
    }

    // se estiver cheia procura uma vitima
    int id_vitima = 0;
    long menorTempo = r->cacheL2[0].ultimoAcesso;

    for (int i = 1; i < TAM_L2; i++) // encontra a (lru)
    {
        if (r->cacheL2[i].ultimoAcesso < menorTempo)
        {
            menorTempo = r->cacheL2[i].ultimoAcesso;
            id_vitima = i;
        }
    }

    // move pra L3
    for (int i = 1; i < TAM_L3; i++) // encontra a (lru)
    {
        if (r->cacheL3[i].dado == valor)
        {
            r->cacheL3[i].tag = r->cacheL2[id_vitima].tag;
            r->cacheL3[i].dado = r->cacheL2[id_vitima].dado;
            r->cacheL3[i].valido = r->cacheL2[id_vitima].valido;
            r->cacheL3[i].ultimoAcesso = r->cacheL2[id_vitima].ultimoAcesso;
        }
    }

    // preenche a cache 2
    r->cacheL2[id_vitima].tag = endereco;
    r->cacheL2[id_vitima].dado = valor;
    r->cacheL2[id_vitima].valido = 1;
    r->cacheL2[id_vitima].ultimoAcesso = r->relogioGlobal;
}

void promoverParaL3(RAM *r, int endereco, int valor)
{
    r->relogioGlobal++; // atualiza o contador de tempo

    // verifica se a cache 3 esta cheia
    for (int i = 0; i < TAM_L3; i++)
    {
        if (!r->cacheL3[i].valido) // se tiver espaco preenche
        {
            r->cacheL3[i].tag = endereco;
            r->cacheL3[i].dado = valor;
            r->cacheL3[i].valido = 1;
            r->cacheL3[i].ultimoAcesso = r->relogioGlobal;
            return;
        }
    }

    // se estiver cheia procura uma vitima
    int id_vitima = 0;
    long menorTempo = r->cacheL3[0].ultimoAcesso;

    for (int i = 1; i < TAM_L3; i++) // encontra a (lru)
    {
        if (r->cacheL3[i].ultimoAcesso < menorTempo)
        {
            menorTempo = r->cacheL3[i].ultimoAcesso;
            id_vitima = i;
        }
    }

    // move pra ram
    simularBuffer(r, r->cacheL3, id_vitima);

    // preenche a cache 3
    r->cacheL3[id_vitima].tag = endereco;
    r->cacheL3[id_vitima].dado = valor;
    r->cacheL3[id_vitima].valido = 1;
    r->cacheL3[id_vitima].ultimoAcesso = r->relogioGlobal;
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

    // alocacao de memoria das cache
    r->cacheL1 = (CacheLine *)malloc(TAM_L1 * sizeof(CacheLine));
    r->cacheL2 = (CacheLine *)malloc(TAM_L2 * sizeof(CacheLine));
    r->cacheL3 = (CacheLine *)malloc(TAM_L3 * sizeof(CacheLine));

    if (!r->cacheL1 || !r->cacheL2 || !r->cacheL3)
    {
        printf("ERRO: Falha ao alocar Caches\n");
    }

    // inicia todas as "linhas" das cache
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

    return buscarNaL1(r, endereco);

}

void setDado(RAM *r, int endereco, int conteudo) // adiciona um dado na ram e nas cache
{
    if (endereco < 0 || endereco >= r->tamanho) // verifica se o endereco eh valido
        return;

    for (int i = 0; i < TAM_L1; i++) // procura na cache 1
    {
        if (r->cacheL1[i].valido && r->cacheL1[i].tag == endereco) // se achar preenche
        {
            r->cacheL1[i].dado = conteudo;
            r->relogioGlobal++;
            r->cacheL1[i].ultimoAcesso = r->relogioGlobal;

            return;
        }
    }

    for (int i = 0; i < TAM_L2; i++) // procura na cache 2
    {
        if (r->cacheL2[i].valido && r->cacheL2[i].tag == endereco)
        {
            r->cacheL2[i].valido = 0; // remove da L2
            promoverParaL1(r, endereco, conteudo);

            return;
        }
    }

    for (int i = 0; i < TAM_L3; i++) // procura na cache 3
    {
        if (r->cacheL3[i].valido && r->cacheL3[i].tag == endereco)
        {
            r->cacheL3[i].valido = 0; // remove da L3
            promoverParaL2(r, endereco, conteudo);
            promoverParaL1(r, endereco, conteudo);

            return;
        }
    }
    promoverParaL3(r, endereco, conteudo);

    promoverParaL2(r, endereco, conteudo);

    promoverParaL1(r, endereco, conteudo);
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
{ // desalloca a memoria da ram e das cache
    free(r->mem);
    free(r->cacheL1);
    free(r->cacheL2);
    free(r->cacheL3);
    free(r);
}

void simularBuffer(RAM *r, CacheLine *Cache3, int id)
{

    r->mem[r->cacheL3[id].tag] = r->cacheL3[id].dado; // a vitima eh enviada de volta pra ram
}

int buscarNaL1(RAM *r, int endereco)
{

    for (int i = 0; i < TAM_L1; i++) // procura em toda a cache 1
    {
        if (r->cacheL1[i].valido && r->cacheL1[i].tag == endereco) // se o endereco nao for vazio e tiver a tag correta
        {
            r->hitsL1++;
            r->relogioGlobal++;                            // aumenta o relogio global
            r->cacheL1[i].ultimoAcesso = r->relogioGlobal; // atualiza o tempo do ultimo acesso
            return r->cacheL1[i].dado;
        }
    }
    
    
    buscarNaL2(r, endereco);
    
    r->hitsL1--;
    
    return buscarNaL1(r, endereco);
}

void buscarNaL2(RAM *r, int endereco)
{
    for (int i = 0; i < TAM_L2; i++) // caso nao encontre na cache 1 procura na 2
    {
        if (r->cacheL2[i].valido && r->cacheL2[i].tag == endereco)
        {
            r->hitsL2++;
            int val = r->cacheL2[i].dado;
            r->cacheL2[i].valido = 0;

            promoverParaL1(r, endereco, val);
            return;
        }
    }
    
    buscarNaL3(r, endereco);
    
    r->hitsL2--;
    
    buscarNaL2(r, endereco);

    return;
}

void buscarNaL3(RAM *r, int endereco)
{
    for (int i = 0; i < TAM_L3; i++) // caso nao encontre na cache 2 procura na 3
    {
        if (r->cacheL3[i].valido && r->cacheL3[i].tag == endereco)
        {
            r->hitsL3++;
            int val = r->cacheL3[i].dado;
            r->cacheL3[i].valido = 0;

            promoverParaL2(r, endereco, val);
            return;
        }
    }

    buscarNaRam(r, endereco);

    r->hitsL3--;
    
    buscarNaL3(r, endereco);

    return;
}

void buscarNaRam(RAM *r, int endereco)
{

    r->missRAM++;
    int val = r->mem[endereco];

    promoverParaL3(r, endereco, val);
    r->mem[endereco] = 0; 

    return;
}