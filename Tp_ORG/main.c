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
    programaDiv(r, c, 20, 4);
    // programaFat(r, c, 4);
    // programaSomaMatriz(r, c, 2);
    // programaRaizQuadrada(r, c, 49);
    // programaRaizCubica(r, c, 27);
    // programaRaioEsfera(r, c, 108);
    // programaBin_Dec(r, c, "1010");
    // programaPA(r, c, 1, 2, 5);
    // programaPotencia(r, c, 3, 4);
    // programaFibonacci(r, c, 0);   //TESTAR 0, 1 e 4
    
    
    
    // programaAreaQuadrado(r, c, 5);
    // programaAreaTriangulo(r, c, 10, 5);
    // programaAreaCirculo(r, c, 10);
    // programaCircunferenciaCirculo(r, c, 10);
    // programaVolumeCaixa(r, c, 3, 4, 5);
    // programaAreaTrapezio(r, c, 10, 6, 5);
    // programaVolumeCilindro(r, c, 10, 5);

    destroiRAM(r);
    destroiCPU(c);

    return 0;
}
