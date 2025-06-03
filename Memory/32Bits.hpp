//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"
#include <sstream>
#include <fstream>
#include <limits>

using namespace std;

//Seleciona parte do bitset (start, end) e converte de decimal para binário
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
    cout << "Pg\t\t\t" << "Frame number" << "\t\t" << "Temporizador" << endl;
    for(int i=0; i<16; i++){
        cout << tlb32[i].pg << "\t\t\t" << tlb32[i].value << "\t\t\t" << tlb16[i].temporizador << endl;
    }

    cout << endl << endl << endl;

    cout << "Frame\t\t" << "Bit valido\t" << "Bit acesso\t" << "Bit dirty" << endl;
    for(int i=0; i<32; i++){
        cout << tp32[i].value << "\t\t" << tp32[i].valid << "\t\t" << tp32[i].access << "\t\t" << tp16[i].dirty << endl;
    }
}

int LRU_tp_32(int frame, TP tp32[]){
    for(int i = 0; i % 32 < 32; i++){ //mod para fazer um for circular, até achar access = 0
        if(!tp32[i].access /*&& !tp32[i].dirty*/){ // 0, 0
            tp32[i].value = frame;
            tp32[i].valid = 1;
            tp32[i].access = 1;
            return i;
        }
        else if(tp32[i].access){
            tp32[i].access = 0;
        }
    }
    return -1;
}

void LRU_tlb_32(int pag, int frame, TLB tlb32[]){
    int posTempoMenor = 0; //menos utilizado
    for (int i = 1; i < 16; i++) {
        if (tlb32[i].temporizador < tlb32[posTempoMenor].temporizador) {
            posTempoMenor = i;
        }
    }
    tlb32[posTempoMenor].pg = pag;
    tlb32[posTempoMenor].value = frame;
    tlb32[posTempoMenor].temporizador++;
}

int findTLB32(TLB tlb32[], int pag, bool &TLBhit){
    for(int i=0; i<16; i++){
        if(tlb32[i].pg == pag){
            TLBhit = true;
            tlb32[i].temporizador++;
            return tlb32[i].value;
        }
    }
    TLBhit = false;
    return -1;
}

int findTP32(TP tp32[], int pag, bool &TPhit){
    if(tp32[pag].valid){
        TPhit = true;
        return tp32[pag].value;
    }

    TPhit = false;
    return -1;
}


/*------------------------------------- 2 Niveis -------------------------------------------*/


void viewTables32_2niveis(TLB tlb32[], NIVEL_1 tp_nivel1[], int pag1){
    cout << "Pg\t\t\t" << "Desloc" << endl;
    for(int i=0; i<16; i++){
        cout << tlb32[i].pg << "\t\t\t" << tlb32[i].value << endl;
    }

    cout << endl << endl << endl;

    cout << "Frame\t\t" << "Bit valido\t" << "Bit acesso\t" << "Bit dirty" << endl;
    for(int i=0; i<32; i++){
        cout << tp_nivel1[pag1].nivel2[i].value << "\t\t" << tp_nivel1[pag1].nivel2[i].valid << "\t\t" << tp_nivel1[pag1].nivel2[i].access << "\t\t" << tp_nivel1[pag1].nivel2[i].dirty << endl;
    }
}

int findTP32_2niveis(NIVEL_1 tp_nivel1[], int pag1, int pag2, bool &TPhit){
    // Primeiro: verifica se o nível 1 está válido
    if (tp_nivel1[pag1].valid) {
        NIVEL_2* tp_nivel2 = tp_nivel1[pag1].nivel2;

        // Se o ponteiro não foi alocado por algum motivo, aloca agora
        if (tp_nivel2 == nullptr) {
            tp_nivel2 = new NIVEL_2[32];
            for (int i = 0; i < 32; ++i) {
                tp_nivel2[i].valid = false;
                tp_nivel2[i].access = false;
                tp_nivel2[i].dirty = false;
            }
            tp_nivel1[pag1].nivel2 = tp_nivel2;
        }

        // Segundo: verifica se o nível 2 está válido
        if (tp_nivel2[pag2].valid) {
            TPhit = true;
            return tp_nivel2[pag2].value;
        }
    }
    else {
        // Se o nível 1 ainda não está válido, aloca o nível 2 e marca como válido
        tp_nivel1[pag1].nivel2 = new NIVEL_2[32];
        for (int i = 0; i < 32; ++i) {
            tp_nivel1[pag1].nivel2[i].valid = false;
        }
        tp_nivel1[pag1].valid = true;
    }

    // Caso não haja acerto (miss)
    TPhit = false;
    return -1;
}

void LRU_tp_2niveis(int frame, NIVEL_1 tp_nivel1[], int &ret_pag1, int &ret_pag2) {
    for (int p1 = 0; p1 < 32; ++p1) {

        if (!tp_nivel1[p1].valid) {
            // Alocar o nível 2 se não estiver alocado
            tp_nivel1[p1].nivel2 = new NIVEL_2[32];

            for (int i = 0; i < 32; ++i) {
                tp_nivel1[p1].nivel2[i].valid = false;
                tp_nivel1[p1].nivel2[i].access = false;
            }

            tp_nivel1[p1].valid = true;
        }

        NIVEL_2* nivel2 = tp_nivel1[p1].nivel2;

        for (int p2 = 0; p2 < 32; ++p2) {

            if (!nivel2[p2].access /* && !nivel2[pag2].dirty */) {
                // Substituição LRU aqui
                nivel2[p2].value = frame;
                nivel2[p2].valid = true;
                nivel2[p2].access = true;

                ret_pag1 = p1;
                ret_pag2 = p2;

                return;
            } 
            else if (nivel2[p2].access) {
                nivel2[p2].access = false;
            }
        }
    }
}
