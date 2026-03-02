#ifndef HD_H
#define HD_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "auxiliares.h"
#include "cpu.h"
#include "programas.h"
#include <stdbool.h>

void escreverHD(void);
bool buscarNoHD(int endereco, int *procuradoHd);
void atualizarHD(int chave, int valor);


#endif