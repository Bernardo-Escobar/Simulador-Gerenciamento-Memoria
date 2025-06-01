//Bernardo Esobar, Laís Blum e Lívia Zimermann

#include <iostream>
#include <bitset>
#include <cstdlib>
#include "structs.h"

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

int pageSum(TLB tlb16[], TP tp16[], int pag, int desloc){
    bool b = true;
    int sum=0;

    for(int i=0; i<16; i++){
        if(pag == tlb16[i].pg){
            sum = tlb16[i].value + desloc;
            b = false;
        }
    }
    if(b){
        for(int i=0; i<32; i++){
            if(pag == tp16[i].pg){
                sum = tp16[i].value + desloc;
            }
        }
    }

    return sum;
}

int main() {

    int pag, desloc, sum=0;
    bool t = true;

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

                    sum = pageSum(tlb16, tp16, pag, desloc);
                break;

                case 2:
                    pag = binaryInt16(num16, 10, 15);
                    desloc = binaryInt16(num16, 0, 9); 

                    sum = pageSum(tlb16, tp16, pag, desloc);
                break;

                case 3:
                    pag = binaryInt16(num16, 11, 15);
                    desloc = binaryInt16(num16, 0, 10); 

                    sum = pageSum(tlb16, tp16, pag, desloc);
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

                    sum = pageSum(tlb32, tp32, pag, desloc);
                break;

                case 2:
                    pag = binaryInt32(num32, 10, 31);
                    desloc = binaryInt32(num32, 0, 9); 

                    sum = pageSum(tlb32, tp32, pag, desloc);
                break;

                case 3:
                    // pag = binaryInt32(num32, 11, 15);
                    // desloc = binaryInt32(num32, 0, 10); 

                    // sum = pageSum(tlb32, tp32, pag, desloc);
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