#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"
#include "cpu.h"

//#include "programas.h"

/*struct programas{
    
    CPU *c;
    RAM *r;


};*/

void programaAleatorio(RAM* ram, CPU* cpu, int qdeIntrucoes);

int main(){
    /*struct programas{

    }*/


    CPU *c = criar_cpu();
    RAM *r = criarRAM_vazia(10);

    programaAleatorio(r,c,15);

    destroiRAM(r);
    destroiCPU(c);
}

/*PROGRAMAS criarPrograma(PROGRAMAS *p,int tam){
    
    p->cpu = criar_cpu();
    p->r-> = criarRAM_vazia(tam);

}*/

void programaAleatorio(RAM* ram, CPU* cpu, int qdeIntrucoes) {
    
    Instrucao* umPrograma = (Instrucao*) malloc(qdeIntrucoes * sizeof(Instrucao));// criar um funçãoc riar instrucao
    
    int tamanhoRAM = 1000;

    srand(time(NULL));

    criarRAM_aleatoria(tamanhoRAM);

    // Loop para preencher o array de instruções
    for (int i = 0; i < (qdeIntrucoes - 1); i++) {

        umPrograma[i].opcode = rand() % 2; 
        
        umPrograma[i].add1 = rand() % tamanhoRAM;
        umPrograma[i].add2 = rand() % tamanhoRAM;
        umPrograma[i].add3 = rand() % tamanhoRAM;
    }

    umPrograma[qdeIntrucoes - 1].opcode = -1;

    setPrograma(cpu, umPrograma);
    iniciar(ram,cpu);

}
