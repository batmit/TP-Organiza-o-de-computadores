#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "Instrucao.h"
#include "ram.h"
#include "cpu.h"


struct cpu { 
     int registrador1;
	 int registrador2;
		
	 int PC;
	 Instrucao *programa;
	 int opcode;
	
};

void setPrograma(CPU *c, Instrucao *programaAux,int tam){
    c->programa[tam].add1 = programaAux[tam].add1;
    c->programa[tam].add2 = programaAux[tam].add2;
    c->programa[tam].add3 = programaAux[tam].add3;
    c->programa[tam].opcode = programaAux[tam].opcode;
 }


CPU* criar_cpu(void) {
    
	CPU* nova_cpu = malloc(sizeof(CPU));
   
    nova_cpu->registrador1 = 0;
    nova_cpu->registrador2 = 0;
    nova_cpu->PC = 0;           
    nova_cpu->opcode = 0;         
    nova_cpu->programa = NULL;  
 
    return nova_cpu;
}

void destroiCPU(CPU *c){
	free(c->programa);
	free(c);
}

void iniciar(RAM *r,CPU *c){
    c->opcode = 0;
    c->PC = 0;
	

    while(c->opcode!=-1){
		c->programa = (Instrucao*) realloc(c->programa, (c->PC + 1) * sizeof(Instrucao));
		c->programa[c->PC].opcode =c->PC;
		c->programa[c->PC].add1 =c->PC; // para motivos de teste e compilação
		if(c->PC == 2)
			c->programa[c->PC].add1 =1;
		if(c->PC == 3)
			c->programa[c->PC].opcode = -1;
        //Instrucao inst = c->programa[c->PC]; /******** */
		Instrucao inst = c->programa[c->PC]; /******** */
		
        c->opcode = inst.opcode;
        switch (c->opcode){
			case -1: {
				printf("Programa terminou!!\n");
				imprimirRAM(r);
				break;
			}
			//soma
			case 0: {
				c->registrador1 = getDado(r,inst.add1);
				c->registrador2 = getDado(r,inst.add2);
				c->registrador1 +=c->registrador2;
				//salvar resultado
				setDado(r,inst.add3, c->registrador1);
				printf("Inst sum -> RAM posicao %d com conteudo %d\n",inst.add3,c->registrador1);
				break;
			}
			//subtrai
			case 1: {
				c->registrador1 = getDado(r,inst.add1);
				c->registrador2 = getDado(r,inst.add2);
				c->registrador1 -= c->registrador2;
				//salvar resultado
				setDado(r,inst.add3, c->registrador1);
				printf("Inst sum -> RAM posicao %d com conteudo %d\n",inst.add3,c->registrador1);
			}
			//copia do registrador para RAM
			//formato da instrucao [opcode,qual_registrador,end_ram,-1]
			case 2: {
				if(inst.add1==1) {
					setDado(r,inst.add2, c->registrador1);                        
					printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n",inst.add2,c->registrador1);
						
				}else if(inst.add1==2) {
					setDado(r,inst.add2, c->registrador2);                        
					printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n",inst.add2,c->registrador2);
						
				}
				break;
			}				
			//copia da RAM para o registrador
			//formato da instrucao [opcode,qual_registrador,end_ram,-1]
			case 3: {
				if(inst.add1==1) {
					c->registrador1 = getDado(r,inst.add2);
                    printf("Inst copy_ram_reg -> Registrador1 com conteudo %d\n",c->registrador1);
					
				}else if(inst.add1==2) {
					c->registrador2 = getDado(r,inst.add2);
					printf("Inst copy_ram_reg -> Registrador2 com conteudo %d\n",c->registrador2);
				}
				break;
			}
			//salva conteudo externo no registrador
			//formato da instrucao [opcode,qual_registrador,conteudo_externo,-1]
			case 4: {
				if(inst.add1==1) {
					c->registrador1 = inst.add2;
					printf("Inst copy_ext_reg -> Registrador1 com conteudo %d\n",c->registrador1);
    
				}else if(inst.add1==2) {
					c->registrador2 = inst.add2;
                    printf("Inst copy_ext_reg -> Registrador2 com conteudo %d\n",c->registrador2);
		
				}
				break;
			}
			//obtem conteudo externo do registrador
			//formato da instrucao [opcode,qual_registrador,-1,-1]
			// note que o conteudo do registrador vai ficar na inst
			//na posicao do 2o endereco, ou seja, [opcode,qual_registrador,conteudo_reg,-1]
			case 5: {
				if(inst.add1==1) {
					inst.add2 = c->registrador1;
                    printf("Inst obtain_reg -> Registrador1 com conteudo %d\n",c->registrador1);
						
				}else if(inst.add1==2) {
					inst.add2 = c->registrador2;
					printf("Inst obtain_reg -> Registrador2 com conteudo %d\n",c->registrador2);
				}
				break;
			}
			
		}				
		c->PC++;
	}
}
