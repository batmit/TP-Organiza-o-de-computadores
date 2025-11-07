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

void programaAleatorio(int qdeIntrucoes);

int main(){
    /*struct programas{

    }*/


    
    

    programaAleatorio(15);

    

}

/*PROGRAMAS criarPrograma(PROGRAMAS *p,int tam){
    
    p->cpu = criar_cpu();
    p->r-> = criarRAM_vazia(tam);

}*/

void programaAleatorio(int qdeIntrucoes) {
    int tamanhoRAM = 1000;
    
    RAM *r = criarRAM_vazia(tamanhoRAM);
    CPU *c = criar_cpu();

    Instrucao* umPrograma = (Instrucao*) malloc(qdeIntrucoes * sizeof(Instrucao));// criar um funçãoc riar instrucao
    srand(time(NULL));



    // Loop para preencher o array de instruções
    for (int i = 0; i < (qdeIntrucoes - 1); i++) {

        umPrograma[i].opcode = rand() % 2; 
        
        umPrograma[i].add1 = rand() % tamanhoRAM;
        umPrograma[i].add2 = rand() % tamanhoRAM;
        umPrograma[i].add3 = rand() % tamanhoRAM;
    }

    umPrograma[qdeIntrucoes - 1].opcode = -1;

    setPrograma(c, umPrograma);
    iniciar(r,c);

    destroiRAM(r);
    destroiCPU(c);

}
