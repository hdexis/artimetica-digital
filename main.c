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
    int offset = n - t;

    for (int i = 0; i < n; i++) {
        saida[i] = 0;
    }

    if (offset < 0) {
        entrada += (t - n); 
        t = n;
        offset = 0;
    }

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

void imprimir_4nBits(int n, int* bits) {
    printf("[ ");
    for(int i=0;i<n;i++) { 
        if(i>0 && i%4==0) printf(" "); 
        printf("%d", bits[i]); 
    }
    printf(" ]");
}

void imprimir_HEX(int n, int* bits) {
    printf("0x");
    for (int i = 0; i < n; i += 4) {
        int val = 8*bits[i] + 4*bits[i+1] + 2*bits[i+2] + 1*bits[i+3];
        
        if (val < 10) printf("%d", val);
        else printf("%c", 'A' + (val - 10)); 
    }
}

void complemento2(int* vetor, int tamanho) {
    // 1. Inverte os bits (Complemento de 1)
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (vetor[i] == 0) ? 1 : 0;
    }
    
    // 2. Soma 1 ao final
    int moeda = 1;
    for (int i = tamanho - 1; i >= 0; i--) {
        int soma = vetor[i] + moeda;
        vetor[i] = soma % 2;
        moeda = soma / 2;
    }
    // O moeda final aqui é descartado propositalmente em complemento de 2
}

int eh_zero(int* vetor, int n) {
    for (int i = 0; i < n; i++) {
        if (vetor[i] != 0) return 0;
    }
    return 1;
}

int maior_ou_igual(int* A, int* B, int n) {
    for (int i = 0; i < n; i++) {
        if (A[i] > B[i]) return 1;
        if (A[i] < B[i]) return 0;
    }
    return 1; // São iguais
}

void addBit(int* R, int n, int novoBit) {
    for (int i = 0; i < n - 1; i++) {
        R[i] = R[i+1];
    }
    R[n - 1] = novoBit;
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

    // Underflow
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
    int B_comp2[n];
    for(int i = 0; i < n; i++) B_comp2[i] = B[i];

    complemento2(B_comp2, n);
    somar_BIN_comS(C, A, B_comp2, n);
    
    if (A[0] != B[0] && C[0] != A[0]) {
        return 0; // Overflow
    }

    return 1; // Sucesso
}

// Exercício 5
void multiplicar_BIN_comS(int* C, int* A, int* B, int n) {
    int A_pos[n];
    int B_pos[n];

    for(int i=0; i<n; i++) {A_pos[i] = A[i]; B_pos[i] = B[i];}
    for(int i=0; i < 2*n; i++) C[i] = 0;

    // XOR dos sinais
    int sinal_C = A[0] ^ B[0]; // 1 diferentes, 0 iguais

    if (A[0] == 1) complemento2(A_pos, n);
    if (B[0] == 1) complemento2(B_pos, n);

    for (int i = n - 1; i >= 0; i--) {
        if (B_pos[i] == 1) {
            int shift = (n - 1) - i;
            int moeda = 0;

            for (int j = n - 1; j >= 0; j--) {
                int idC = (2 * n - 1) - shift - ((n - 1) - j);
                
                if (idC >= 0) {
                    int soma = C[idC] + A_pos[j] + moeda;
                    C[idC] = soma % 2;
                    moeda = soma / 2;
                }
            }

            int k = (2 * n - 1) - shift - n;
            while (moeda > 0 && k >= 0) {
                int soma = C[k] + moeda;
                C[k] = soma % 2;
                moeda = soma / 2;
                k--;
            }
        }
    }

    if (sinal_C == 1) {
        complemento2(C, 2 * n);
    }
}

// Exercício 6
int dividir_BIN_semS(int* C, int* R, int* A, int* B, int n) {
    // Divisão por Zero
    if (eh_zero(B, n)) return 0;

    for (int i = 0; i < n; i++) {
        C[i] = 0;
        R[i] = 0;
    }

    int tempSub[n];

    // MSB (0) ao LSB (n-1)
    for (int i = 0; i < n; i++) {
        
        addBit(R, n, A[i]);

        if (maior_ou_igual(R, B, n)) {
            subtrair_BIN_semS(tempSub, R, B, n);
            
            for(int k=0; k<n; k++) R[k] = tempSub[k];
            C[i] = 1;
        } else {
            C[i] = 0;
        }
    }
    return 1;
}

// Exercício 7
int bcd_to_int(int* bits) {
    return 8*bits[0] + 4*bits[1] + 2*bits[2] + 1*bits[3];
}

void int_to_bcd(int val, int* bits) {
    bits[3] = val % 2; val /= 2;
    bits[2] = val % 2; val /= 2;
    bits[1] = val % 2; val /= 2;
    bits[0] = val % 2;
}

void somar_BCD_semS(int* C, int* A, int* B, int n) {
    int num_digitos = n / 4;
    int moeda = 0;

    for(int k=0; k < n + 4; k++) C[k] = 0;

    for (int i = num_digitos - 1; i >= 0; i--) {
        int id = i * 4; 

        int intA = bcd_to_int(&A[id]);
        int intB = bcd_to_int(&B[id]);

        int soma = intA + intB + moeda;

        if (soma > 9) {
            soma += 6;
            moeda = 1; 
        } else {
            moeda = 0;
        }

        int_to_bcd(soma, &C[id + 4]); 
    }

    int_to_bcd(moeda, &C[0]);
}

// Exercício 8
int subtrair_BCD_semS(int* C, int* A, int* B, int n) {
    int num_digitos = n / 4;
    int moeda = 0;
    for(int i=0; i < n; i++) C[i] = 0;

    for (int i = num_digitos - 1; i >= 0; i--) {
        int id = i * 4;

        int intA = bcd_to_int(&A[id]);
        int intB = bcd_to_int(&B[id]);

        int diff = intA - intB - moeda;

        if (diff < 0) {
            diff += 10; 
            moeda = 1; 
        } else {
            moeda = 0;
        }

        int_to_bcd(diff, &C[id]);
    }

    if (moeda != 0) {
        return 0;
    }
    
    return 1;
}

// Exercício 9
int somar_HEX_comS(int* C, int* A, int* B, int n) {
    int moeda = 0;
    
    for (int i = n - 1; i >= 0; i--) {
        int soma = A[i] + B[i] + moeda;
        C[i] = soma % 2;
        moeda = soma / 2;
    }
    
    
    if (A[0] == B[0] && C[0] != A[0]) {
        return 0; // Overflow
    }

    return 1;
}

// Exercício 10
int subtrair_HEX_comS(int* C, int* A, int* B, int n) {
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
    if (A[0] != B[0] && C[0] != A[0]) {
        return 0; // Overflow
    }

    return 1;
}


int main() {
    int Smax = 128; // Tamanho do buffer
    char buffer[Smax];
    char bufferA[Smax];
    char bufferB[Smax];

    int exercicio = 0;
    printf("Número do exercício: "); ler(buffer); exercicio = atoi(buffer); printf("\n");
    printf("A = ");ler(bufferA); int lenA = strlen(bufferA);
    printf("B = ");ler(bufferB); int lenB = strlen(bufferB); 

    int n = lenA;
    if (lenB > lenA) n = lenB;
    if (n == 0) n = 1; 

     if ((exercicio >= 7 && exercicio <= 10) && n % 4 != 0) {
        n += (4 - (n % 4));
    }

    int A[n];
    int B[n];
    string_nBits(bufferA, n, A);
    string_nBits(bufferB, n, B);
    int C[2*n];
    int R[n];

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
                printf("\033[31mERRO: O resultado seria binário negativo !!!\033[0m\n\n");
            }
            break;
    
        case 3:
            printf("\n\033[33m--- [3] Soma Binária (Complemento de 2) ---\033[0m\n");
        
            printf("A        "); imprimir_nBits(n, A); printf(" (%c)\n", A[0] ? '-' : '+');
            printf("B        "); imprimir_nBits(n, B); printf(" (%c)\n", B[0] ? '-' : '+');

            if (somar_BIN_comS(C, A, B, n) == 1) {
                printf("A + B    ");imprimir_nBits(n, C); printf(" (%c)\n", C[0] ? '-' : '+');
            } else {
                printf("\033[31mERRO: Overflow !!!\033[0m\n\n");
            }
            break;

        case 4:
            printf("\n\033[33m--- [4] Subtração Binária (Complemento de 2) ---\033[0m\n");
            printf("A        "); imprimir_nBits(n, A); printf(" (%c)\n", A[0] ? '-' : '+');
            printf("B        "); imprimir_nBits(n, B); printf(" (%c)\n", B[0] ? '-' : '+');

            if (subtrair_BIN_comS(C, A, B, n) == 1) {
                printf("A - B    "); imprimir_nBits(n, C); printf(" (%c)\n", C[0] ? '-' : '+');
            } else {
                printf("\033[31mERRO: Overflow !!!\033[0m\n\n");
            }
            break;
        
        case 5:
            printf("\n\033[33m--- [5] Multiplicação Binária (Complemento de 2) ---\033[0m\n");
            printf("A        "); imprimir_nBits(n, A); printf(" (%c)\n", A[0] ? '-' : '+');
            printf("B        "); imprimir_nBits(n, B); printf(" (%c)\n", B[0] ? '-' : '+');
            
            multiplicar_BIN_comS(C, A, B, n);
            printf("A * B    "); imprimir_nBits(2*n, C); printf(" (%c)\n\n", C[0] ? '-' : '+');
            break;

        case 6:
            printf("\n\033[33m--- [6] Divisão Binária (Sem Sinal) ---\033[0m\n");
            printf("A        "); imprimir_nBits(n, A); printf("\n");
            printf("B        "); imprimir_nBits(n, B); printf("\n");

            if (dividir_BIN_semS(C, R, A, B, n) == 1) {
                printf("A / B    "); imprimir_nBits(n, C); printf("\n");
                printf("A %% B    "); imprimir_nBits(n, R); printf("\n\n");
            } else {
                printf("\033[31mERRO: Divisão por Zero !!!\033[0m\n\n");
            }
            break;

        case 7:
            printf("\n\033[33m--- [7] Soma BCD (Sem Sinal) ---\033[0m\n");
            
            printf("A        "); imprimir_4nBits(n, A); printf("\n");
            printf("B        "); imprimir_4nBits(n, B); printf("\n");

            somar_BCD_semS(C, A, B, n);

            printf("A + B    ");  imprimir_4nBits(n+4, C); printf("\n\n");
            break;
        
        case 8:
            printf("\n\033[33m--- [8] Subtração BCD (Sem Sinal) ---\033[0m\n");
            
            printf("A        "); imprimir_4nBits(n, A); printf("\n");
            printf("B        "); imprimir_4nBits(n, B); printf("\n");

            if (subtrair_BCD_semS(C, A, B, n)==1) {
                printf("A - B    ");  imprimir_4nBits(n, C); printf("\n\n");
            } else {
                printf("----------------------------------------\n");
                printf("\033[31mERRO: O resultado seria BCD negativo !!!\033[0m\n\n");
            }
            break;

        case 9:
            printf("\n\033[33m--- [9] Soma Hexadecimal (Complemento de 2) ---\033[0m\n");
            
            printf("A (BIN)      "); imprimir_4nBits(n, A); printf("    ");
            printf("A (HEX)      "); imprimir_HEX(n, A); printf("\n");

            printf("B (BIN)      "); imprimir_4nBits(n, B); printf("    ");
            printf("B (HEX)      "); imprimir_HEX(n, B); printf("\n");

            if (somar_HEX_comS(C, A, B, n) == 1) {
                printf("A + B (BIN)  "); imprimir_4nBits(n, C); printf("    ");
                printf("A + B (HEX)  ");imprimir_HEX(n, C); printf("\n\n");
            } else {
                printf("\033[31mERRO: Overflow !!!\033[0m\n\n");
            }
            break;
            
        case 10:
            printf("\n\033[33m--- [10] Subtração Hexadecimal (Complemento de 2) ---\033[0m\n");
            
            printf("A (BIN)      "); imprimir_4nBits(n, A); printf("    ");
            printf("A (HEX)      "); imprimir_HEX(n, A); printf("\n");

            printf("B (BIN)      "); imprimir_4nBits(n, B); printf("    ");
            printf("B (HEX)      "); imprimir_HEX(n, B); printf("\n");

            if (subtrair_HEX_comS(C, A, B, n) == 1) {
                printf("A - B (BIN)  "); imprimir_4nBits(n, C); printf("    ");
                printf("A - B (HEX)  ");imprimir_HEX(n, C); printf("\n\n");
            } else {
                printf("\033[31mERRO: Overflow !!!\033[0m\n\n");
            }
            break;

    default:
        printf("Entrada inválida!\n");
}
return 0;
}
