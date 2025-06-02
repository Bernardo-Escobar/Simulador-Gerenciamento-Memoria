//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"
#include "16Bits.hpp"
#include "32Bits.hpp"

#include <fstream>
#include <limits>

using namespace std;

fstream& gotoLine(fstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}

int pageSum32(TLB tlb32[], TP tp32[], int pag, int desloc){
    bool b = true;
    int sum=0;

    for(int i=0; i<32; i++){
        if(pag == tlb32[i].pg){
            sum = tlb32[i].value + desloc;
            b = false;
        }
    }
    if(b){
        sum = tp32[pag].value + desloc;
    }

    return sum;
}


int main() {

    int pag, desloc, frame = 0, valorMemo = 0;
    int TLBhitCount = 0, TPhitCount = 0;
    bool TLBhit;

    string valorFinal;

    int op1=0, op2=0;

    cout << "Qual tamanho de endereco virtual? 1 - 16 bits; 2 - 32 bits" << endl;
    cin >> op1;

    switch(op1){
        case 1:{
            bitset<16> num16(59757);

            cout << "Qual tamanho do deslocamento? 1 - 256 B; 2 - 1 KB; 3 - 2 KB" << endl;
            cin >> op2;

            switch(op2){
                case 1:
                    pag = binaryInt16(num16, 8, 15);
                    desloc = binaryInt16(num16, 0, 7); 

                    frame = procuraTLB16(tlb16, pag, TLBhit);

                    if(TLBhit){
                        TLBhitCount++;

                        valorMemo = frame * 256 + desloc;
                    }
                    else{
                        frame = procuraTP16(tlb16, pag, TPhit);

                        if(TPhit){
                            TPhitCount++;

                            valorMemo = frame * 256 + desloc;
                        }
                        else{
                           frame = lerBackingStore(pag);

                           swap(frame, tp16);
                        }
                    }

                break;

                case 2:
                    pag = binaryInt16(num16, 10, 15);
                    desloc = binaryInt16(num16, 0, 9); 

                    sum = pageSum16(tlb16, tp16, pag, desloc);
                break;

                case 3:
                    pag = binaryInt16(num16, 11, 15);
                    desloc = binaryInt16(num16, 0, 10); 

                    sum = pageSum16(tlb16, tp16, pag, desloc);
                break;
            }

            viewTables16(tlb16, tp16);

            cout << num16 << endl;
        break;
        }

        case 2:{
            bitset<32> num32(4064679778);

            cout << "Qual tamanho do deslocamento? 1 - 256 B; 2 - 1 KB; 3 - 4 KB" << endl;
            cin >> op2;

            switch(op2){
                case 1:
                    pag = binaryInt32(num32, 8, 31);
                    desloc = binaryInt32(num32, 0, 7); 

                    sum = pageSum32(tlb32, tp32, pag, desloc);
                break;

                case 2:
                    pag = binaryInt32(num32, 10, 31);
                    desloc = binaryInt32(num32, 0, 9); 

                    sum = pageSum32(tlb32, tp32, pag, desloc);
                break;

                case 3:
                    // pag = binaryInt32(num32, 11, 15);
                    // desloc = binaryInt32(num32, 0, 10); 

                    // sum = pageSum32(tlb32, tp32, pag, desloc);
                break;
            }

            viewTables32(tlb32, tp32);

            cout << num32 << endl;

        break;
        }
    }

    

    fstream file("data_memory.txt");
    gotoLine(file, sum);
    
    file >> valorFinal;



    cout << endl << endl;

    //cout << "Endereco virtual: " << num << endl;
    cout << "Pagina: " << pag << endl;
    cout << "Deslocamento: " << desloc << endl;
    // Acao tomada
    cout << "Valor lido: " << valorFinal << endl;

    cin.get();

    return 0;
}