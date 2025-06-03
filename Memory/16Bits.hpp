//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"

#include <fstream>
#include <limits>

using namespace std;

//Seleciona parte do bitset (start, end) e converte de binário para decimal
int binaryInt16(const bitset<16>& bits, int start, int end) {
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


void viewTables16(TLB tlb16[], TP tp16[]){
    cout << "Pg\t\t" << "Frame number" << "\t\t" << "Temporizador" << endl;
    for(int i=0; i<16; i++){
        cout << tlb16[i].pg << "\t\t" << tlb16[i].value << "\t\t\t" << tlb16[i].temporizador << endl;
    }

    cout << endl << endl << endl;

    cout << "Frame\t\t" << "Bit valido\t" << "Bit acesso\t" << "Bit dirty" << endl;
    for(int i=0; i<32; i++){
        cout << tp16[i].value << "\t\t" << tp16[i].valid << "\t\t" << tp16[i].access << "\t\t" << tp16[i].dirty << endl;
    }
}

int LRU_tp_16(int frame, TP tp16[]){
    for(int i = 0; i % 32 < 32; i++){ //mod para fazer um for circular, até achar access = 0
        if(!tp16[i].access /*&& !tp16[i].dirty*/){ // 0, 0
            tp16[i].value = frame;
            tp16[i].valid = 1;
            tp16[i].access = 1;
            return i;
        }
        else if(tp16[i].access){
            tp16[i].access = 0;
        }
    }
    return -1;
}

void LRU_tlb_16(int pag, int frame, TLB tlb16[]){
    int posTempoMenor = 0; //menos utilizado
    for (int i = 1; i < 16; i++) {
        if (tlb16[i].temporizador < tlb16[posTempoMenor].temporizador) {
            posTempoMenor = i;
        }
    }
    tlb16[posTempoMenor].pg = pag;
    tlb16[posTempoMenor].value = frame;
    tlb16[posTempoMenor].temporizador++;
}

int findTLB16(TLB tlb16[], int pag, bool &TLBhit){
    for(int i=0; i<16; i++){
        if(tlb16[i].pg == pag){
            TLBhit = true;
            tlb16[i].temporizador++;
            return tlb16[i].value;
        }
    }
    TLBhit = false;
    return -1;
}

int findTP16(TP tp16[], int pag, bool &TPhit){
    if(tp16[pag].valid){
        TPhit = true;
        return tp16[pag].value;
    }

    TPhit = false;
    return -1;
}

