#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Instrucao.h"
#include "ram.h"
#include "cpu.h"
#include "programas.h"

int main()
{

    srand(time(NULL));

    RAM *r = criarRAM_vazia(50);
    CPU *c = criar_cpu();

    // programaAleatorio(r, c, 15);
    // programaMult(r, c, 20, 5);
    // programaDiv(r, c, 20, 4);
    // programaFat(r, c, 4);
    // programaSomaMatriz(r, c, 2);
    // programaRaizQuadrada(r, c, 49);
    // programaRaizCubica(r, c, 27);
    // programaRaioEsfera(r, c, 108);
    programaBinParaDec(r, c, 10)

        destroiRAM(r);
    destroiCPU(c);

    return 0;
}
