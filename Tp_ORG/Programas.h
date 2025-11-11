#ifndef PROGRAMAS_H
#define PROGRAMAS_H

#include "ram.h"
#include "cpu.h"

/*
    Atenção, escreva o layot da ram, para facil utilização das funções
    e pra entendermos os codigos de todos
*/

// programas de maior logica para utilizar em funções
void programaAleatorio(RAM *ram, CPU *cpu, int qdeIntrucoes);
void programaFat(RAM *ram, CPU *cpu, int fat);
void programaMult(RAM *ram, CPU *cpu, int multiplicando, int multiplicador);
void programaDiv(RAM *ram, CPU *cpu, int dividendo, int divisor);
void programaSomaMatriz(RAM *ram, CPU *cpu, int cardinalidade);
void programaRaizQuadrada(RAM *ram, CPU *cpu, int numero);
void programaRaizCubica(RAM *ram, CPU *cpu, int numero);
void programaRaioEsfera(RAM *ram, CPU *cpu, int volume);
void programaDivFloat2(RAM *ram, CPU *cpu, int dividendo, int divisor); 
void programaMMC(RAM *ram, CPU *cpu, int a, int b);
void programaBin_Dec(RAM *ram, CPU *cpu, char *binario);
void programaDec_Bin(RAM *ram, CPU *cpu, int decimal);

void programaHex_Bin(RAM *ram, CPU *cpu,char *hex);
void programaHex_Dec(RAM *ram, CPU *cpu,char *hex);

void programaBin_Hex(RAM *ram, CPU *cpu,char *binario);
void programaDec_Hex(RAM *ram, CPU *cpu, int decimal);

void programaSomaPA(RAM *ram, CPU *cpu, int a1, int razao, int n);
void programaExpBinaria(RAM *ram, CPU *cpu, int base, int expoente);

void programaAreaQuadrado(RAM *ram, CPU *cpu, int lado);
void programaAreaTriangulo(RAM *ram, CPU *cpu, int base, int altura);
void programaAreaCirculo(RAM *ram, CPU *cpu, int raio);
void programaCircunferenciaCirculo(RAM *ram, CPU *cpu, int raio);
void programaVolumeCaixa(RAM *ram, CPU *cpu, int comprimento, int largura, int altura);
void programaAreaTrapezio(RAM *ram, CPU *cpu, int baseMaior, int baseMenor, int altura);
void programaVolumeCilindro(RAM *ram, CPU *cpu, int raio, int altura);




void programaPA(RAM *ram, CPU *cpu, int a1, int razao, int n);
void programaPotencia(RAM *ram, CPU *cpu, int base, int expoente);
void programaFibonacci(RAM *ram, CPU *cpu, int n);
void programaMultMatriz(RAM *ram, CPU *cpu, int N);


//Daniel
void programaPG(RAM *ram, CPU *cpu, int firstValue, int razao, int numValues);
void programaMdc(RAM *ram, CPU *cpu, int n, int k);
void programaRestoDiv(RAM *ram, CPU *cpu, int n, int k);
void programaLog(RAM *ram, CPU *cpu, int base, int valor);
// void determinante(RAM *ram, CPU *cpu);


#endif