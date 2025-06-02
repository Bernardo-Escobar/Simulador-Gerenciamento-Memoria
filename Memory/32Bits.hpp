//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"

#include <fstream>
#include <limits>

using namespace std;

int binaryInt32(const bitset<32>& bits, int start, int end) {
    int result = 0; // Armazena o valor final em inteiro
    int bitPosition = 0; // Posição atual do bit no resultado final

    for (int i = start; i <= end; ++i) {
        if (bits[i]) { // Verifica se o bit na posição i é 1
            result |= (1 << bitPosition); // Define o bit correspondente em result
        }

        ++bitPosition; // Avança para a próxima posição do bit no resultado
    }

    return result;
}

void viewTables32(TLB tlb32[], TP tp32[]){
    cout << "Pg\t\t" << "Desloc\t\t" << endl;
    for(int i=0; i<16; i++){
        cout << tlb32[i].pg << "\t\t" << tlb32[i].value << endl;
    }

    cout << endl << endl << endl;

    cout << "Pg\t\t" << "Desloc\t\t" << "Bit valido\t" << "Bit acesso" << endl;
    for(int i=0; i<32; i++){
        cout << tp32[i].pg << "\t\t" << tp32[i].value << "\t\t" << tp32[i].valid << "\t\t" << tp32[i].access << endl;
    }
}