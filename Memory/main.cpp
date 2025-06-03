//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"
#include "16Bits.hpp"
#include "32Bits.hpp"
#include <math.h> //para usar potência

#include <fstream>
#include <limits>

#include <sstream>

using namespace std;

fstream& gotoLine(fstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}

int findBackingStore(int searchPag) {
    ifstream arquivo("backing_store.txt");
    string linha;

    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string paginaStr, frameStr;

        if (getline(ss, paginaStr, ';') and getline(ss, frameStr)) {
            int pag = stoi(paginaStr);
            int frame = stoi(frameStr);
    
            if (pag == searchPag){
                return frame;
            }
        }
    }

    return -1;
}

int main() {

    int pag, desloc, frame = 0, valorMemo = 0;
    int TLBhitCount = 0, TPhitCount = 0;
    bool TLBhit;
    bool TPhit;
    int sum, tam;
    int pos;
    unsigned int endVirtual; //valor máximo 4294967295 (2^32 - 1)
    int pag1, pag2;
    int op_entrada; //hex ou dec
    bool sair = false;

    string valorFinal;

    int op1=0, op2=0;
    bool eh_4KB = false;

    while(!sair){

        cout << "Qual tamanho de endereco virtual? 1 - 16 bits; 2 - 32 bits" << endl;
        cin >> op1;

        cout << "Vai informar o endereço em que formato? 1 - Decimal; 2 - Hexadecimal" << endl;
        cin >> op_entrada;

        switch(op1){
            case 1:{
                if(op_entrada == 2){
                    string endString;
                    do{
                        cout << "Qual é o endereço virtual? (Entre 0 e FFFF)" << endl;
                        cin >> endString;
                        endVirtual = stoi(endString, nullptr, 16);  // Base 16 = hexadecimal
                    } while(endVirtual < 0 || endVirtual > pow(2, 16) - 1);
                }
                else{
                    do{
                        cout << "Qual é o endereço virtual? (Entre 0 e 65535)" << endl;
                        cin >> endVirtual;
                    } while(endVirtual < 0 || endVirtual > pow(2, 16) - 1);
                }

                // exemplos:
                // (256B) 1130     --> 0000010001101010 --> não acha na TLB --> acha na tp (198)
                // (1KB)  59757 --> 1110100101101101 --> acha na TLB (198)
                // (2KB)  3218  --> 0000110010010010 --> não acha na TLB --> não acha na tp

                bitset<16> num16(endVirtual);

                cout << "Qual tamanho do deslocamento? 1 - 256 B; 2 - 1 KB; 3 - 2 KB" << endl;
                cin >> op2;

                switch(op2){
                    case 1:
                        tam = 256;
                        pag = binaryInt16(num16, 8, 15);   //00000100--------  --> 4
                        desloc = binaryInt16(num16, 0, 7); //--------01101010  --> 106
                    break;

                    case 2:
                        tam = 1024;
                        pag = binaryInt16(num16, 10, 15);  //111010---------- --> 58
                        desloc = binaryInt16(num16, 0, 9); //------0101101101 --> 365
                    break;

                    case 3:
                        tam = 2048;
                        pag = binaryInt16(num16, 11, 15);
                        desloc = binaryInt16(num16, 0, 10);
                    break;
                }

                frame = findTLB16(tlb16, pag, TLBhit);

                if(!TLBhit){
                    frame = findTP16(tp16, pag, TPhit);

                    if(!TPhit){ // swap
                        frame = findBackingStore(pag);
                        if(frame != -1){
                            pos = LRU_tp_16(frame, tp16);

                            LRU_tlb_16(pag, tp16[pos].value, tlb16);

                            frame = findTLB16(tlb16, pag, TLBhit);
                            TLBhit = false; // para a saída não indicar que foi encontrada na TLB, mas sim no backing store

                            sum = frame * tam + desloc;
                        }
                        else{
                            cout << "Erro ao tentar achar no backing store" << endl;
                        }
                    }
                    else{ // encontrou na TP
                        tp16[pag].access = 1;

                        LRU_tlb_16(pag, tp16[pag].value, tlb16);

                        frame = findTP16(tp16, pag, TPhit);

                        sum = frame * tam + desloc;

                    }
                }
                else{ // encontrou na TLB
                    sum = frame * tam + desloc;
                }

                viewTables16(tlb16, tp16);

                cout << endl << endl;

                cout << "Endereco virtual: " << num16 << endl;
            break;
            }

            case 2:{
                if(op_entrada == 2){
                    string endString;
                    do{
                        cout << "Qual é o endereço virtual? (Entre 0 e FFFFFFFF)" << endl;
                        cin >> endString;
                        endVirtual = stoi(endString, nullptr, 16);  // Base 16 = hexadecimal
                    } while(endVirtual < 0 || endVirtual > pow(2, 32) - 1);
                }
                else{
                    do{
                        cout << "Qual é o endereço virtual? (Entre 0 e 4294967295)" << endl;
                        cin >> endVirtual;
                    } while(endVirtual < 0 || endVirtual > pow(2, 32) - 1);
                }

                // exemplos:
                // (256B) 669    --> 00000000000000000000001010011101 --> não acha na TLB --> acha na tp (211)
                // (1KB)  945453 --> 00000000000011100110110100101101 --> acha na TLB (923)
                // (4KB)  26309  --> 00000000000000000110011011000101 --> não acha na TLB --> não acha na tp

                bitset<32> num32(endVirtual);

                cout << "Qual tamanho do deslocamento? 1 - 256 B; 2 - 1 KB; 3 - 4 KB" << endl;
                cin >> op2;

                switch(op2){
                    case 1:
                        tam = 256;
                        pag = binaryInt32(num32, 8, 31);
                        desloc = binaryInt32(num32, 0, 7);
                    break;

                    case 2:
                        tam = 1024;
                        pag = binaryInt32(num32, 10, 31);
                        desloc = binaryInt32(num32, 0, 9); 
                    break;

                    case 3:
                        tam = 4096;
                        pag1 = binaryInt32(num32, 22, 31);
                        pag2 = binaryInt32(num32, 12, 21);
                        desloc = binaryInt32(num32, 0, 11);

                        eh_4KB = true;
                    break;
                }

                

                if(!eh_4KB){
                    frame = findTLB32(tlb32, pag, TLBhit);
                    
                    if(!TLBhit){
                        frame = findTP32(tp32, pag, TPhit);

                        if(!TPhit){ // swap
                            frame = findBackingStore(pag);
                            if(frame != -1){
                                pos = LRU_tp_32(frame, tp32);

                                LRU_tlb_32(pag, tp32[pos].value, tlb32);

                                frame = findTLB32(tlb32, pag, TLBhit);
                                TLBhit = false; // para a saída não indicar que foi encontrada na TLB, mas sim no backing store

                                sum = frame * tam + desloc;
                            }
                            else{
                                cout << "Erro ao tentar achar no backing store" << endl;
                            }
                        }
                        else{ // encontrou na TP
                            tp32[pag].access = 1;

                            LRU_tlb_32(pag, tp32[pag].value, tlb32);

                            frame = findTP32(tp32, pag, TPhit);

                            sum = frame * tam + desloc;
                        }
                    }
                    else{ // encontrou na TLB
                        sum = frame * tam + desloc;
                    }

                    viewTables32(tlb32, tp32);
                }
                else{
                    frame = findTLB32(tlb32, pag2, TLBhit);

                    if(!TLBhit){
                        frame = findTP32_2niveis(tp_nivel1, pag1, pag2, TPhit);

                        if(!TPhit){ // swap
                            frame = findBackingStore(pag2);
                            
                            if(frame != -1){
                                int ret_pag1 = 0, ret_pag2 = 0;

                                LRU_tp_2niveis(frame, tp_nivel1, ret_pag1, ret_pag2);

                                LRU_tlb_32(pag2, tp_nivel1[ret_pag1].nivel2[ret_pag2].value, tlb32);

                                frame = findTLB32(tlb32, pag2, TLBhit);
                                TLBhit = false; // para a saída não indicar que foi encontrada na TLB, mas sim no backing store

                                sum = frame * tam + desloc;
                            }
                            else{
                                cout << "Erro ao tentar achar no backing store" << endl;
                            }
                        }
                        else{ // encontrou na TP
                            tp_nivel1[pag1].nivel2[pag2].access = 1;

                            LRU_tlb_32(pag2, tp_nivel1[pag1].nivel2[pag2].value, tlb32);

                            frame = findTP32(tp32, pag, TPhit);

                            sum = frame * tam + desloc;
                        }
                    }
                    else{ // encontrou na TLB
                        sum = frame * tam + desloc;
                    }

                    viewTables32_2niveis(tlb32, tp_nivel1, pag1);
                }            

                cout << endl << endl;
                cout << "Endereco virtual: " << num32 << endl;

            break;
            }
        }

        

        fstream file("data_memory.txt");
        gotoLine(file, sum);
        
        file >> valorFinal;

        if(TLBhit){
            cout << "TLBhit: " << boolalpha << TLBhit << endl;
            cout << "Carregado da TLB" << endl;
        }
        else if(TPhit){
            cout << "TLBhit: " << boolalpha << TLBhit << " | TPhit: " << boolalpha << TPhit << endl;
            cout << "Carregado da TP" << endl;
        }
        else if (!valorFinal.empty()){
            cout << "TLBhit: " << boolalpha << TLBhit << " | TPhit: " << boolalpha << TPhit << endl;
            cout << "Carregado da backing store!" << endl;
        }
        else{
            cout << "Valor não encontrado" << endl;
        }
        
        if(eh_4KB){
            cout << "Pagina 1: " << pag1 << endl;
            cout << "Pagina 2: " << pag2 << endl;
        }
        else{
            cout << "Pagina: " << pag << endl;
        }
        
        cout << "Deslocamento: " << desloc << endl;
        cout << "Soma: " << sum << endl;
        cout << "Valor lido: " << valorFinal << endl;

        cin.get();

        TLBhit = false;
        TPhit = false;

        cout << endl << endl;
        cout << "Deseja sair? 1 - Sim; 0 - Não" << endl; 
        cin >> sair;
        cout << endl << endl;
    }

    return 0;
}
