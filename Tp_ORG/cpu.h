#ifndef CPU_H
#define CPU_H

#include "ram.h"
#include "auxiliares.h"
#include<time.h>

typedef struct cpu CPU;
typedef struct interrupcao Interrupcao;
void setPrograma(CPU *c, Instrucao *programaAux);
CPU *criar_cpu(void);
void iniciar(RAM *r, CPU *c);
void destroiCPU(CPU *c);
void tratarInterrupcao(RAM *r, CPU *c, int nivel);

void executarInstrucao(RAM *r, CPU *c, Instrucao inst);


#endif