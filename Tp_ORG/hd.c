#include "hd.h"


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
int buscarHD(int n){
    FILE *arq = fopen("hd.bin", "rb");
    if (arq == NULL) {
        return;
    } 

    int i, valor;

    while (fread(&i, sizeof(int), 1, arq) == 1) {

        if (fread(&valor, sizeof(int), 1, arq) != 1)
            break;

        if (i == n) {
            fclose(arq);
            return valor;
        }
    }

    fclose(arq);
    return -1; // não encontrado
}

//Atualiza o que está no HD considerando os valores da RAM
void atualizarHD(RAM *r){
    FILE *arq = fopen("hd.bin", "wb");
    if (arq == NULL) {
        return;
    }
    
    while(fread(&i, sizeof(int), 1, arq) == 1){

        for(int j = 0; j < )


    }



}