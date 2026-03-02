#include "hd.h"
#include "ram.h"


//                  Estrutura
//{Chave palavra} sequencialmente, sem nenhum intermediário
//A chave é o índice numérico do valor, que vai de 0 a 255

//inicializa o HD com valores aleatórios
void escreverHD(){
    FILE *arq = fopen("hd.bin", "wb");
    if (arq == NULL) {
        return;
    }

    int n;

    for(int i = 0; i < 256; i++){

        n = rand() % 1000;
        fwrite(&i, sizeof(int), 1, arq);
        fwrite(&n, sizeof(int), 1, arq);


    }


    fclose(arq);

}

//Busca no HD o valor a partir do índice
bool buscarNoHD(int endereco, int *procuradoHd){
    FILE *arq = fopen("hd.bin", "rb");
    if (arq == NULL) {
        return false;
    } 

    int i, valor;

    while (fread(&i, sizeof(int), 1, arq) == 1) {

        if (fread(&valor, sizeof(int), 1, arq) != 1)
            break;

        if (i == endereco) {
            *procuradoHd = valor;
            fclose(arq);
            *procuradoHd = valor;
            return true;
        }
    }

    fclose(arq);
    return false; // não encontrado
}

//Atualiza o que está no HD considerando os valores da RAM
void atualizarHD(int chave, int valor){

    FILE *arq = fopen("hd.bin", "rb+"); // leitura e escrita sem apagar
    if (!arq) return;

    int k, v;

    while (1) {
        // Lê a chave
        if (fread(&k, sizeof(int), 1, arq) != 1) break;

        // Lê o valor
        if (fread(&v, sizeof(int), 1, arq) != 1) break;

        if (k == chave) {
            // Voltamos o cursor para "em cima" do valor que acabamos de ler
            if (fseek(arq, -(long)sizeof(int), SEEK_CUR) != 0) {
                fclose(arq);
                return;
            }

            // Sobrescreve somente o valor
            if (fwrite(&valor, sizeof(int), 1, arq) != 1) {
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