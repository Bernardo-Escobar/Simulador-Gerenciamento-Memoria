# 🧠 Simulador de Gerenciamento de Memória — Trabalho M2

Repositório desenvolvido como parte da Avaliação 03 da disciplina de **Sistemas Operacionais** (2025/1) do curso de **Ciência da Computação** da **Universidade do Vale do Itajaí (UNIVALI)**.  
O projeto simula mecanismos de **memória principal** e **memória virtual**, com suporte a **TLB**, **page faults**, **bit de validade**, e mais.

---

## 🎯 Objetivo

Simular o funcionamento de um sistema de gerenciamento de memória, capaz de traduzir endereços virtuais em físicos usando conceitos como:

- Paginação com deslocamento
- TLB com substituição LRU
- Paginação hierárquica (para endereços de 32 bits)
- Tratamento de faltas de página
- Acesso à backing store
- Bits de validade, acesso e modificação

---

## 📂 Estrutura do Projeto

```bash
Trabalho-M2/
├── Memory/                 # Código-fonte do simulador de memória
│   ├── Relatorio M2.pdf
│   ├── main.cpp            # Entrada principal do simulador
│   ├── structs.cpp         # Estruturas e manipulações auxiliares
│   ├── structs.h
│   ├── 16bits.hpp
│   ├── 32bits.hpp       
│   ├── data_memory.txt     # Memória simulada
│   └── backing_store.txt   # Armazenamento secundário (simulado)
├── IPC/                  
├── Threads/               
├── Scheduler/            
├── Process/                
├── File System/            
├── Time execution/         
└── README.md               # Este documento
```

## ⚙️ Como Compilar e Executar
💻 Navegar até a pasta Memory
```bash
cd Memory
```
🔧 Compilar
```bash
g++ main.cpp structs.cpp -o main -lpthread -lrt
```
▶️ Executar
```bash
./main
```

## 🔍 Exemplo de Saída
```yaml
Endereço virtual: 19986
Número da página: 4
Deslocamento: 3602
TLB miss
Page hit
Valor lido: 50
```

## ✅ Funcionalidades Implementadas
 Tradução de endereços virtuais (16 e 32 bits)

 Paginação com deslocamento (256B, 1KB, 4KB)

 Leitura de endereço via linha de comando ou arquivo

 Simulação de backing store (carregamento por demanda)

 Bits de validade, acesso e modificação (valid, accessed, dirty)

 TLB com até 16 entradas (substituição LRU)

 Tratamento de page faults

## 📚 Conceitos Abordados
Gerência de memória (real e virtual)

Paginação simples e hierárquica

TLB e substituição LRU

Acesso concorrente à memória

Manipulação de bits com operadores bitwise

Threads e sincronização (pthread)

## 📊 Relatório
O relatório completo do projeto encontra-se em PDF.

## 👨‍💻 Autores
Trabalho realizado por alunos da Universidade do Vale do Itajaí (UNIVALI)
Disciplina: Sistemas Operacionais – Avaliação M2
Professor: Felipe Viel

## 📜 Licença
Uso acadêmico e educacional. Livre para consulta e aprendizado.
