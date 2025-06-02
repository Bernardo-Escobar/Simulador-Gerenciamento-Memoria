//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"

#include <fstream>
#include <limits>

using namespace std;

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
    cout << "Pg\t\t" << "Desloc\t\t" << endl;
    for(int i=0; i<16; i++){
        cout << tlb16[i].pg << "\t\t" << tlb16[i].value << endl;
    }

    cout << endl << endl << endl;

    cout << "Pg\t\t" << "Desloc\t\t" << "Bit valido\t" << "Bit acesso" << endl;
    for(int i=0; i<32; i++){
        cout << tp16[i].pg << "\t\t" << tp16[i].value << "\t\t" << tp16[i].valid << "\t\t" << tp16[i].access << endl;
    }
}

int procuraTLB16(TLB tlb16[], int pag, bool &TLBhit){
    for(int i=0; i<16; i++){
        if(tlb16[i].pg == pag){
            TLBhit = true;
            return tlb16[i].pg;
        }
    }
    TLBhit = false;
    return nullptr;
}

int procuraTP16(TP tp16[], int pag, bool &TPhit){
    if(tp16[pag].valid){
        return tp16[pag].value;
        TPhit = true;
    }

    TPhit = false;
    return nullptr;
}

int lerBackingStore(int paginaBuscada) {
    ifstream arquivo("backing_store.txt");
    string linha;

    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string paginaStr, frameStr;

        if (getline(ss, paginaStr, '; ') and getline(ss, frameStr)) {
            int pag = stoi(paginaStr);
            int frame = stoi(frameStr);
    
            if (pag == paginaBuscada) {
                return frame;
            }
        }
    }

    return nullptr;
}

void swap(int frame, TP tp16[]){
    for(int i = 0; i < 32; i++){
        if(!tp16[i].access && !tp16[i].dirty){
            tp16[i].value = frame;
            tp16[i].valid = 1;
            tp16[i].access = 1;
        }
        else if(!tp16[i].access /*&& tp16[i].dirty*/){
            tp16[i].value = frame;
            tp16[i].valid = 1;
            tp16[i].access = 1;
        }
    }
}