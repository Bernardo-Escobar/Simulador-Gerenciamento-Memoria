#ifndef STRUCTS_H
    #define STRUCTS_H

    using namespace std;

    // Definição da estrutura usada na comunicação
    typedef struct {
        int pg; // Numero da pagina
        int value; // valor associado
    } TLB;

    typedef struct {
        int pg; // Numero da pagina
        int value; // valor associado
        bool valid;
        bool access;
        // bool dirty;
    } TP;

    extern TLB tlb16[16];
    extern TP tp16[32];

#endif
   