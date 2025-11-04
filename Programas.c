#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"
#include "cpu.h"

int main(){
    CPU *c = criar_cpu();
    RAM *r = criarRAM_vazia(10);

    iniciar(r,c);
    imprimirRAM(r);

    destroiRAM(r);
    destroiCPU(c);
}