#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//macro de leitura
#define ler(xxx) __ler(xxx, sizeof(xxx))

void __ler(char* entrada, int tamanho) {
    fflush(stdout);
    if (fgets(entrada, tamanho, stdin) != NULL) {
        // Truque para remover o '\n' do final da string
        entrada[strcspn(entrada, "\n")] = '\0';
    } else {
        // Se der erro na leitura, esvazia a string
        entrada[0] = '\0';
    }
}

void string_nBits(char* entrada, int n, int* saida) {
    int t = strlen(entrada);
    int offset = n - t; // Calcula quantos zeros precisam ser adicionados à esquerda

    // Limpa o vetor de saída com zeros antes de começar
    for (int i = 0; i < n; i++) {
        saida[i] = 0;
    }

    if (offset < 0) {
        entrada += (t - n); 
        t = n;
        offset = 0;
    }

    // Char -> Int
    for (int i = 0; i < t; i++) {
        char c = entrada[i];
        if (entrada[i] == '1') {
            saida[offset + i] = 1;
        } else {
            saida[offset + i] = 0; 
        }
    }
}

void imprimir_nBits(int n, int* bits) {
    printf("[ ");
    for(int i = 0; i < n; i++) {
        printf("%d ", bits[i]);
    }
    printf("]");
}

void complemento2(int* vetor, int tamanho) {
    // 1. Inverte os bits (Complemento de 1)
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (vetor[i] == 0) ? 1 : 0;
    }
    
    // 2. Soma 1 ao final
    int carry = 1;
    for (int i = tamanho - 1; i >= 0; i--) {
        int soma = vetor[i] + carry;
        vetor[i] = soma % 2;
        carry = soma / 2;
    }
    // O carry final aqui é descartado propositalmente em complemento de 2
}

// Exercício 1
int somar_BIN_semS(int* C, int* A, int* B, int n) {
    int moeda = 0;
    for (int i = n - 1; i >= 0; i--) {
        int soma = A[i] + B[i] + moeda;
        C[i + 1] = soma % 2;
        moeda = soma / 2;
    }
    C[0] = moeda;
    return 1;
}

// Exercício 2
int subtrair_BIN_semS(int* C, int* A, int* B, int n) {
    int moeda = 0;

    // Percorre do bit menos significativo (final) para o mais significativo
    for (int i = n - 1; i >= 0; i--) {
        int sub = A[i] - B[i] - moeda;

        if (sub < 0) {
            sub = sub + 2; // Pega emprestado da base 2 (vira 10 em binário, que é 2)
            moeda = 1;    // Marca que pegou emprestado
        } else {
            moeda = 0;    // Não precisou pedir emprestado
        }
        
        C[i] = sub;
    }

    // Se no final ainda tiver moeda, significa que B > A (Underflow)
    if (moeda != 0) {
        return 0;
    }
    
    return 1;
}

// Exercício 3
int somar_BIN_comS(int* C, int* A, int* B, int n) {
    int moeda = 0;
    for (int i = n - 1; i >= 0; i--) {
        int soma = A[i] + B[i] + moeda;
        C[i] = soma % 2;
        moeda = soma / 2;
    }
    
    // Verificar sinal
    if (A[0] == B[0] && C[0] != A[0]) {
        return 0; // Overflow
    }

    return 1;
}

// Exercício 4
int subtrair_BIN_comS(int* C, int* A, int* B, int n) {
    int moeda = 0;
    for (int i = n - 1; i >= 0; i--) {
        int sub = A[i] - B[i] - moeda;
        if (sub < 0) {
            sub = sub + 2;
            moeda = 1;
        } else {
            moeda = 0;
        }
        C[i] = sub;
    }

    // 2. A verificação de erro (Overflow) é diferente
    // O erro só acontece se os sinais forem DIFERENTES.
    // Se A é (+), B é (-), A-B tem que dar (+). Se der (-), estourou.
    // Se A é (-), B é (+), A-B tem que dar (-). Se der (+), estourou.
    
    if (A[0] != B[0] && C[0] != A[0]) {
        return 0; // Overflow
    }

    return 1; // Sucesso
}

// Exercício 5
void multiplicar_BIN_comS(int* C, int* A, int* B, int n) {
    // Arrays temporários para guardar os valores positivos
    int tempA[n];
    int tempB[n];
    
    // Copia os valores originais
    for(int i=0; i<n; i++) { tempA[i] = A[i]; tempB[i] = B[i]; }

    // Limpa o array C (que tem tamanho 2*n) com zeros
    for(int i=0; i < 2*n; i++) C[i] = 0;

    // 1. Determina o sinal do resultado (XOR dos sinais)
    int sinalResultado = A[0] ^ B[0]; // 1 se sinais diferentes, 0 se iguais

    // 2. Transforma em positivo (Módulo) se for negativo
    if (A[0] == 1) complemento2(tempA, n);
    if (B[0] == 1) complemento2(tempB, n);

    // 3. Multiplicação "Shift and Add" (Algoritmo clássico)
    // Percorre B da direita (LSB) para a esquerda
    for (int i = n - 1; i >= 0; i--) {
        if (tempB[i] == 1) {
            // Se o bit de B for 1, somamos A ao acumulador C, deslocado corretamente
            int shift = (n - 1) - i; // Quantas casas deslocar para esquerda
            int carry = 0;

            // Soma tempA ao C nas posições corretas
            // O LSB de A (índice n-1) vai somar no C na posição (2n-1) - shift
            for (int j = n - 1; j >= 0; j--) {
                int idxC = (2 * n - 1) - shift - ((n - 1) - j);
                
                // Cuidado para não acessar índice negativo (embora com 2n sobre n não deva acontecer)
                if (idxC >= 0) {
                    int soma = C[idxC] + tempA[j] + carry;
                    C[idxC] = soma % 2;
                    carry = soma / 2;
                }
            }

            // Propaga o carry restante para a esquerda em C
            int k = (2 * n - 1) - shift - n;
            while (carry > 0 && k >= 0) {
                int soma = C[k] + carry;
                C[k] = soma % 2;
                carry = soma / 2;
                k--;
            }
        }
    }

    // 4. Se o resultado deveria ser negativo, aplica complemento de 2 no vetor C inteiro (2n bits)
    if (sinalResultado == 1) {
        complemento2(C, 2 * n);
    }
}

// Exercício 6
int dividir_BIN_semS() {

}

// Exercício 7
int somar_BCD_semS() {

}

// Exercício 8
int subtrair_BCD_semS() {

}

// Exercício 9
int somar_HEX_comS() {

}

// Exercício 10
int subtrair_HEX_comS() {

}


int main() {
    int Smax = 128; // Tamanho do buffer
    char buffer[Smax];
    char bufferA[Smax];
    char bufferB[Smax];
    int A[Smax];
    int B[Smax];

    int exercicio = 0;
    printf("Número do exercício: "); ler(buffer); exercicio = atoi(buffer); printf("\n");
    printf("A = ");ler(bufferA); int lenA = strlen(bufferA);
    printf("B = ");ler(bufferB); int lenB = strlen(bufferB); 

    int n = lenA;
    if (lenB > lenA) n = lenB;
    if (n == 0) n = 1; 

    string_nBits(bufferA, n, A);
    string_nBits(bufferB, n, B);
    int C[2*n];

    switch (exercicio) {

        case 1:
            printf("\n\033[33m--- [1] Soma Binária (Sem Sinal) ---\033[0m\n");
            somar_BIN_semS(C, A, B, n);

            printf("A        "); imprimir_nBits(n, A); printf("\n");
            printf("B        "); imprimir_nBits(n, B); printf("\n");
            printf("A + B    "); imprimir_nBits(n+1, C);printf("\n\n");
            break;
    
        case 2:
            printf("\n\033[33m--- [2] Sutração Binária (Sem Sinal) ---\033[0m\n");

            printf("A        "); imprimir_nBits(n, A); printf("\n");
            printf("B        "); imprimir_nBits(n, B); printf("\n");

            if (subtrair_BIN_semS(C, A, B, n) == 1) {
                printf("A - B    "); imprimir_nBits(n, C);printf("\n\n");
            } else {
                printf("\033[31mERRO: O resultado seria binário negativo!!!\033[0m\n\n");
            }
            break;
    
        case 3:
            printf("\n\033[33m--- [3] Soma Binária (Com Sinal) (Complemento de 2) ---\033[0m\n");
        
            printf("A        "); imprimir_nBits(n, A); printf(" (Sinal: %d)\n", A[0]);
            printf("B        "); imprimir_nBits(n, B); printf(" (Sinal: %d)\n", B[0]);

            if (somar_BIN_comS(C, A, B, n) == 1) {
                printf("A + B    ");imprimir_nBits(n, C); printf(" (Sinal: %d)\n\n", C[0]);
            } else {
                printf("\033[31mERRO: Overflow (Estouro de Sinal)!\033[0m\n");
                printf("A soma de dois numeros com mesmo sinal resultou no sinal oposto.\n");
            }
            break;

        case 4:
            printf("\n\033[33m--- [4] Subtração Binária (Com Sinal) ---\033[0m\n");
            printf("A        "); imprimir_nBits(n, A); printf(" (Sinal: %d)\n", A[0]);
            printf("B        "); imprimir_nBits(n, B); printf(" (Sinal: %d)\n", B[0]);

            if (subtrair_BIN_comS(C, A, B, n) == 1) {
                printf("A - B    "); imprimir_nBits(n, C); printf(" (Sinal: %d)\n\n", C[0]);
            } else {
                printf("\033[31mERRO: Overflow (Estouro de Sinal)!\033[0m\n");
                printf("Subtração de numeros com sinais opostos estourou o limite.\n\n");
            }
            break;
        
        case 5:
            printf("\n\033[33m--- [5] Multiplicação Binária (Com Sinal) ---\033[0m\n");
            printf("A          "); imprimir_nBits(n, A); printf(" (%d)\n", A[0] ? -1 : 1); // Indicação visual simples
            printf("B          "); imprimir_nBits(n, B); printf(" (%d)\n", B[0] ? -1 : 1);
            
            // Chama a função
            multiplicar_BIN_comS(C, A, B, n);

            printf("----------------------------------------\n");
            printf("A * B      "); 
            // O resultado tem tamanho 2*n
            imprimir_nBits(2 * n, C); 
            printf("\n\n");
            break;

    default:
        printf("Entrada inválida!\n");
}
return 0;
}