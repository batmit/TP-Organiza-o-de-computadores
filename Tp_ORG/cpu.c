#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "cpu.h"
#include "auxiliares.h"
#include "programas.h"

struct interrupcao
{

	int pc;
	Instrucao *instrucoes;
	int valido;
};

struct cpu
{
	int registrador1;
	int registrador2;
	int PC;
	Instrucao *programa;
	int opcode;

	Interrupcao interromper[3];
};

void setPrograma(CPU *c, Instrucao *programaAux)
{
	if (c->programa != NULL)
	{
		free(c->programa);
	}

	c->programa = programaAux;
}

CPU *criar_cpu(void)
{

	CPU *nova_cpu = malloc(sizeof(CPU));

	nova_cpu->registrador1 = 0;
	nova_cpu->registrador2 = 0;
	nova_cpu->PC = 0;
	nova_cpu->opcode = 0;
	nova_cpu->programa = NULL;

	// nova_cpu->interromper = malloc(3 * sizeof(Interrupcao));

	return nova_cpu;
}

void destroiCPU(CPU *c)
{
	if (c->programa != NULL)
	{
		free(c->programa);
	}
	free(c);
}

void iniciar(RAM *r, CPU *c)
{
	// 	/* OPCODE
	// 	-1 = Halt
	// 	0 soma
	// 	1 subtrai
	// 	2 copia do registrador para RAM
	// 	3 copia da RAM para o registrador
	// 	4 salva conteudo externo no registrador
	// 	5 obtem conteudo externo do registrador
	// 	*/

	c->PC = 0;
	c->opcode = 0;

	printf("[CPU] Iniciando execução do programa principal...\n");

	while (c->opcode != -1)
	{

		if (c->programa == NULL)
		{
			printf("[ERRO] Nenhum programa carregado na CPU.\n");
			break;
		}

		Instrucao inst = c->programa[c->PC];
		c->opcode = inst.opcode;

		if (c->opcode != -1)
		{
			executarInstrucao(r, c, inst);

			c->PC++;
		}
		else
		{
			printf("[CPU] Comando HALT encontrado. Encerrando...\n");
			break;
		}

		if ((rand() % 300) < 5)
		{
			printf("\n[SISTEMA] >>> Sinal de interrupção externa no PC: %d <<<\n", c->PC);

			int i = rand() % 3;

			c->interromper[i].instrucoes = c->programa;
			c->interromper[i].pc = c->PC;


			c->interromper[i].valido = 1;

			tratarInterrupcao(r, c, i);
			c->interromper[i].valido = 0;

			c->opcode = c->programa[c->PC].opcode;
		}
	}

	printf("[CPU] Ciclo de execução finalizado.\n");
}


void tratarInterrupcao(RAM *r, CPU *c, int nivel)
{
	// EMPILHAR

	int pcOriginal = c->PC;
	Instrucao *programaOriginal = c->programa;
	int r1Original = c->registrador1;
	int r2Original = c->registrador2;

	printf("\n[CPU] >>> Entrando no Tratador Nível %d <<<\n", nivel + 1);

	// carregar dados
	c->programa = c->interromper[nivel].instrucoes;
	c->PC = 0;
	c->opcode = 0;
	c->interromper[nivel].valido = 0;
	c->programa = programaInt(r, c, 40);

	while (c->opcode != -1)
	{
		// Se houver uma interrupção de maior prioridade (casa 0),

		for (int j = 0; j < nivel; j++)
		{
			if (c->interromper[j].valido)
			{
				tratarInterrupcao(r, c, j);
			}
		}

		Instrucao inst = c->programa[c->PC];
		c->opcode = inst.opcode;

		// Executa a instrução (Soma, Sub, etc.)
		executarInstrucao(r, c, inst);

		c->PC++;
	}

	free(c->programa);

	// DESEMPILHAR
	printf("[CPU] <<< Saindo do Nível %d. Voltando contexto anterior...\n", nivel + 1);
	c->programa = programaOriginal;
	c->PC = pcOriginal;
	c->registrador1 = r1Original;
	c->registrador2 = r2Original;
	c->opcode = 0;
}

void executarInstrucao(RAM *r, CPU *c, Instrucao inst)
{
	switch (c->opcode)
	{
	case -1:
	{
		printf("Programa terminou!!\n");
		// imprimirRAM(r);
		break;
	}
		// soma
	case 0:
	{
		c->registrador1 = getDado(r, inst.add1);
		c->registrador2 = getDado(r, inst.add2);
		c->registrador1 += c->registrador2;
		// salvar resultado
		setDado(r, inst.add3, c->registrador1);
		// printf("Inst sum -> RAM posicao %d com conteudo %d\n", inst.add3, c->registrador1);
		break;
	}
		// subtrai
	case 1:
	{
		c->registrador1 = getDado(r, inst.add1);
		c->registrador2 = getDado(r, inst.add2);
		c->registrador1 -= c->registrador2;
		// salvar resultado
		setDado(r, inst.add3, c->registrador1);
		// printf("Inst sub -> RAM posicao %d com conteudo %d\n", inst.add3, c->registrador1);

		break;
	}
	// copia do registrador para RAM
	// formato da instrucao [opcode,qual_registrador,end_ram,-1]
	case 2:
	{
		if (inst.add1 == 1)
		{
			setDado(r, inst.add2, c->registrador1);
			// printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n", inst.add2, c->registrador1);
		}
		else if (inst.add1 == 2)
		{
			setDado(r, inst.add2, c->registrador2);
			// printf("Inst copy_reg_ram -> RAM posicao %d com conteudo %d\n", inst.add2, c->registrador2);
		}
		break;
	}
	// copia da RAM para o registrador
	// formato da instrucao [opcode,qual_registrador,end_ram,-1]
	case 3:
	{
		if (inst.add1 == 1)
		{
			c->registrador1 = getDado(r, inst.add2);
			// printf("Inst copy_ram_reg -> Registrador1 com conteudo %d\n", c->registrador1);
		}
		else if (inst.add1 == 2)
		{
			c->registrador2 = getDado(r, inst.add2);
			// printf("Inst copy_ram_reg -> Registrador2 com conteudo %d\n", c->registrador2);
		}
		break;
	}
	// salva conteudo externo no registrador
	// formato da instrucao [opcode,qual_registrador,conteudo_externo,-1]
	case 4:
	{
		if (inst.add1 == 1)
		{
			c->registrador1 = inst.add2;
			// printf("Inst copy_ext_reg -> Registrador1 com conteudo %d\n", c->registrador1);
		}
		else if (inst.add1 == 2)
		{
			c->registrador2 = inst.add2;
			// printf("Inst copy_ext_reg -> Registrador2 com conteudo %d\n", c->registrador2);
		}
		break;
	}
	// obtem conteudo externo do registrador
	// formato da instrucao [opcode,qual_registrador,-1,-1]
	//  note que o conteudo do registrador vai ficar na inst
	// na posicao do 2o endereco, ou seja, [opcode,qual_registrador,conteudo_reg,-1]
	case 5:
	{
		if (inst.add1 == 1)
		{
			c->programa[c->PC].add2 = c->registrador1;
			// printf("Inst obtain_reg -> Registrador1 com conteudo %d\n", c->registrador1);
		}
		else if (inst.add1 == 2)
		{
			c->programa[c->PC].add2 = c->registrador2;
			// printf("Inst obtain_reg -> Registrador2 com conteudo %d\n", c->registrador2);
		}

		break;
	}
	}
}

Instrucao *programaInt(RAM *ram, CPU *cpu, int qdeIntrucoes)
{

	Instrucao *umPrograma = (Instrucao *)malloc(qdeIntrucoes * sizeof(Instrucao));

	for (int i = 0; i < (qdeIntrucoes - 1); i++)
	{
		umPrograma[i].opcode = rand() % 2;
		umPrograma[i].add1 = rand() % 256;
		umPrograma[i].add2 = rand() % 256;
		umPrograma[i].add3 = rand() % 256;
	}

	umPrograma[qdeIntrucoes - 1].opcode = -1;

	return umPrograma;
}