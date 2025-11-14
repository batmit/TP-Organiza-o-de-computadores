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
     // programaDiv(r, c, 25, 3);
    // programaFat(r, c, 4);
    // programaSomaMatriz(r, c, 2);
    // programaRaizQuadrada(r, c, 49);
    // programaRaizCubica(r, c, 27);
    // programaRaioEsfera(r, c, 108);
    //  programaDivFloat2(r, c, 25, 3);
    // programaMMC(r, c, 15, 10);
     
    // programaBin_Dec(r, c, "1010");
    // programaDec_Bin(r, c, 5);
    // programaHex_Bin(r,c,"512A");
    // programaHex_Dec(r,c,"512A");
    // programaBin_Hex(r,c,"101000100101010");
    // programaDec_Hex(r, c, 20778);

    // programaSomaPA(r , c, 1,  1,  10);
    // programaExpBinaria(r,c,2,5);

    // programaPrimo(r, c , 643);


    // programaBhaskara(r, c , 1,  5 , 3);

    // programaAreaQuadrado(r, c, 5);
    // programaAreaTriangulo(r, c, 10, 5);
    // programaAreaCirculo(r, c, 10);
    // programaCircunferenciaCirculo(r, c, 10);
    // programaVolumeCaixa(r, c, 3, 4, 5);
    // programaAreaTrapezio(r, c, 10, 6, 5);
    // programaVolumeCilindro(r, c, 10, 5);

    // programaPA(r, c, 1, 2, 5);
    // programaPotencia(r, c, 3, 4);
    // programaFibonacci(r, c, 0);
    // programaMultMatriz(r, c, 3)

    // programaLog(r, c, 3, 27); 
    
    
    // programaPG(r, c, 2, 3, 4); // nao funciona

    // programaRestoDiv(r, c, 7, 3);
    // programaMdc(r, c, 25, 60);

    // determinante(r , c); 

    destroiRAM(r);
    destroiCPU(c);
    printf("\n\nEXECUÇÃO ENCERRADA !!!\n\n");

    return 0;
}
