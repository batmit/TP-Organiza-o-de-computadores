#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ram.h"
#include "auxiliares.h"
#include "cpu.h"
#include "programas.h"
#include <stdbool.h>

void escreverHD(RAM *r){

    FILE *arq = fopen("hd.bin", "ab");
    if (arq == NULL) {
        return;
    }




}