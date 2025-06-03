#ifndef STRUCTS_H
    #define STRUCTS_H

    using namespace std;

    // Definição da estrutura usada na comunicação
    typedef struct {
        int pg; // Numero da pagina
        int value; // valor associado
        int temporizador = 0;
    } TLB;

    typedef struct {
        int value; // valor associado
        bool valid;
        bool access;
        bool dirty;
    } TP;

    typedef struct {
        int value; // valor associado
        bool valid;
        bool access;
        bool dirty;
    } NIVEL_2;

    typedef struct {
        NIVEL_2* nivel2; // valor associado
        bool valid;
    } NIVEL_1; 

    extern TLB tlb16[16];
    extern TP tp16[32];
    
    extern TLB tlb32[16];
    extern TP tp32[32];

    extern NIVEL_1 tp_nivel1[32];

#endif
   