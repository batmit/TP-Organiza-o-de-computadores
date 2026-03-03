#include "hd.h"
#include "ram.h"

//                  Estrutura
//{Chave palavra} sequencialmente, sem nenhum intermediário
// A chave é o índice numérico do valor, que vai de 0 a 255

// inicializa o HD com valores aleatórios
void escreverHD()
{
    FILE *arq = fopen("hd.bin", "wb");
    if (arq == NULL)
    {
        return;
    }

    int n;

    for (int i = 0; i < 256; i++)
    {

        n = rand() % 1000;
        fwrite(&i, sizeof(int), 1, arq);
        fwrite(&n, sizeof(int), 1, arq);
    }

    fclose(arq);
}

// Busca no HD o valor a partir do índice
bool buscarNoHD(int endereco, int *procuradoHd)
{
    FILE *arq = fopen("hd.bin", "rb");
    if (arq == NULL)
    {
        return false;
    }

    int i, valor;

    while (fread(&i, sizeof(int), 1, arq) == 1)
    {

        if (fread(&valor, sizeof(int), 1, arq) != 1)
            break;

        if (i == endereco)
        {
            *procuradoHd = valor;
            fclose(arq);
            *procuradoHd = valor;
            return true;
        }
    }

    fclose(arq);
    return false; // não encontrado
}

// Atualiza o que está no HD considerando os valores da RAM
void atualizarHD(int chave, int valor)
{

    FILE *arq = fopen("hd.bin", "rb+"); // leitura e escrita sem apagar
    if (!arq)
        return;

    int k, v;

    while (1)
    {
        // Lê a chave
        if (fread(&k, sizeof(int), 1, arq) != 1)
            break;

        // Lê o valor
        if (fread(&v, sizeof(int), 1, arq) != 1)
            break;

        if (k == chave)
        {
            // Voltamos o cursor para "em cima" do valor que acabamos de ler
            if (fseek(arq, -(long)sizeof(int), SEEK_CUR) != 0)
            {
                fclose(arq);
                return;
            }

            // Sobrescreve somente o valor
            if (fwrite(&valor, sizeof(int), 1, arq) != 1)
            {
                fclose(arq);
                return;
            }

            fflush(arq);
            fclose(arq);
            return;
        }
    }

    fclose(arq);
    return;
}

void imprimirHD() {
    FILE *arq = fopen("hd.bin", "rb");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo do HD");
        return;
    }

    int chave, valor;
    int idx = 0;

    printf("=== Conteudo de %s ===\n", "hd.bin");
    printf("Formato: (chave -> valor)\n\n");

    // lê pares (chave, valor) até acabar o arquivo
    while (1) {
        size_t l1 = fread(&chave, sizeof(int), 1, arq);
        size_t l2 = fread(&valor, sizeof(int), 1, arq);

        if (l1 == 1 && l2 == 1) {
            printf("[%03d] %d -> %d\n", idx, chave, valor);
            idx++;
        } else {
            break; // EOF ou arquivo truncado
        }
    }

    if (!feof(arq)) {
        // Se não foi EOF, pode ter dado erro de leitura ou arquivo corrompido/truncado
        perror("Aviso: leitura interrompida (arquivo pode estar incompleto)");
    }

    printf("\nTotal de registros lidos: %d\n", idx);

    fclose(arq);
}

void imprimirDARAM(RAM *r) {
    if (r == NULL) {
        printf("RAM nao inicializada.\n");
        return;
    }

    printf("=========== RAM ===========\n");
    printf("Tamanho: %d\n", r->tamanho);
    printf("Relogio Global: %ld\n", r->relogioGlobal);
    printf("Hits L1: %d | Hits L2: %d | Hits L3: %d\n",
           r->hitsL1, r->hitsL2, r->hitsL3);
    printf("----------------------------\n");

    if (r->mem == NULL) {
        printf("Memoria principal nao alocada.\n");
        return;
    }

    for (int i = 0; i < r->tamanho; i++) {
        printf("[%03d] = %d: %d\n", i, r->mem[i].chave, r->mem[i].valor);
    }

    printf("============================\n");
}