#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "auxiliares.h"
#include "cpu.h"
#include "programas.h"
#include <stdbool.h>

//                  Estrutura
//{Chave palavra} sequencialmente, sem nenhum intermediário
//A chave é o índice numérico do valor, que vai de 0 a 255
//
//
//
//
//
void escreverHD(RAM *r){

    FILE *arq = fopen("hd.bin", "ab");
    if (arq == NULL) {
        return;
    }




}