#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "cpu.h"
#include "programas.h"
#include "auxiliares.h"

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

    //  programaRaizCubica(r, c, 27);
    //  programaRaioEsfera(r, c, 108);

    //  programaDivFloat2(r, c, 6, 10);
    //  programaMMC(r, c, 15, 10);
    //  programaBin_Dec(r, c, "1010");
    //  programaDec_Bin(r, c, 5);
    // programaHex_Bin(r,c,"0x5");
    // programaHex_Dec(r,c,"0x44A");

    // programaPA(r, c, 1, 2, 5);
    // programaPotencia(r, c, 3, 4);
    // programaFibonacci(r, c, 0);
    // programaMultMatriz(r, c, 3)

    // programaLog(r, c, 2, 8); AINDA NÃO FUNCIONAL

    // programaPG(r, c, 2, 2, 6); AINDA NÃO FUNCIONAL
    // programaRestoDiv(r, c, 7, 3);
    // programaMdc(r, c, 25, 60);
    // programaAreaQuadrado(r, c, 5);
    // programaAreaTriangulo(r, c, 10, 5);
    // programaAreaCirculo(r, c, 10);
    // programaCircunferenciaCirculo(r, c, 10);
    // programaVolumeCaixa(r, c, 3, 4, 5);
    // programaAreaTrapezio(r, c, 10, 6, 5);
    // programaVolumeCilindro(r, c, 10, 5);

    destroiRAM(r);
    destroiCPU(c);
    printf("\n\nEXECUÇÃO ENCERRADA !!!\n\n");

    return 0;
}
