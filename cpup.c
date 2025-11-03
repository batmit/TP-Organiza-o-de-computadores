#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"

int main(){
    RAM *r = criarRAM_vazia(5);
    imprimir(r);
}