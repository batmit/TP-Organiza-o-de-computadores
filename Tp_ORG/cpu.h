#ifndef CPU_H
#define CPU_H

typedef struct cpu CPU;

void setPrograma(CPU *c, Instrucao *programaAux);
CPU* criar_cpu(void);
void iniciar(RAM *r,CPU *c);
void destroiCPU(CPU *c);

#endif