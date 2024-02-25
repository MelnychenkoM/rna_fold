#include "rna_fold.h"
#include <string.h>
#include <stdio.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

// Nussinov algorithm for rna folding
void nussinov(const char rna[], char* structure) {
    int length = strlen(rna);

    printf("Input sequence: %s\n", rna);
    printf("The length of the rna sequence: %d\n", length);

    int matrix[length][length];

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            matrix[i][j] = 0;
        }
    }

    for (int j = 0; j < length; j++) {
        for (int i = 0; i < j; i++) { 
            matrix[i][j] = max(matrix[i][j], matrix[i+1][j]);
            if ((rna[i] == 'A' && rna[j] == 'U') || (rna[i] == 'U' && rna[j] == 'A') ||
                (rna[i] == 'C' && rna[j] == 'G') || (rna[i] == 'G' && rna[j] == 'C')) {
                matrix[i][j] = max(matrix[i][j], matrix[i+1][j-1] + 1);
            }
            for (int k = i+1; k < j-1; k++){
                matrix[i][j] = max(matrix[i][j], matrix[i][k] + matrix[k+1][j]);
            }
        }
    }

    for (int i = 0; i < length; i++) {
        structure[i] = '.'; 
    }
    structure[length] = '\0'; 

    int i = 0;
    int j = length - 1;
    while (i < j) {
        if ((rna[i] == 'A' && rna[j] == 'U') || (rna[i] == 'U' && rna[j] == 'A') ||
            (rna[i] == 'C' && rna[j] == 'G') || (rna[i] == 'G' && rna[j] == 'C')) {
            if (matrix[i][j] == matrix[i + 1][j - 1] + 1) {
                structure[i] = '(';
                structure[j] = ')';
                i++;
                j--;
                continue;
            }
        }
        if (matrix[i][j] == matrix[i + 1][j]) {
            i++;
        } else {
            j--;
        }
    }
    // printf("Optimal secondary structure: %s\n", structure);
}

// Nussinov matrix
void populate_matrix(const char rna[], int length, int matrix[length][length]) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            matrix[i][j] = 0;
        }
    }

    for (int j = 0; j < length; j++) {
        for (int i = 0; i < j; i++) {
            matrix[i][j] = max(matrix[i][j], matrix[i + 1][j]);
            if ((rna[i] == 'A' && rna[j] == 'U') || (rna[i] == 'U' && rna[j] == 'A') ||
                (rna[i] == 'C' && rna[j] == 'G') || (rna[i] == 'G' && rna[j] == 'C')) {
                matrix[i][j] = max(matrix[i][j], matrix[i + 1][j - 1] + 1);
            }
            for (int k = i + 1; k < j - 1; k++) {
                matrix[i][j] = max(matrix[i][j], matrix[i][k] + matrix[k + 1][j]);
            }
        }
    }
}

double GC_content(const char* seq){
    int length = strlen(seq);

    int G_count = 0;
    int C_count = 0;
    int A_count = 0;
    int U_count = 0;
    int T_count = 0;

    for (int i = 0; i < length; i++){
        switch (seq[i]) {
        case 'A':
            A_count++;
            break;

        case 'C':
            C_count++;
            break;
        
        case 'U':
            U_count++;
            break;

        case 'G':
            G_count++;
            break;

        case 'T':
            T_count++;
            break;
        }
    }
    
    int GC_count = G_count + C_count;
    int total_count = GC_count + A_count + U_count + T_count;

    // Avoid division by zero
    if (total_count == 0) {
        return 0.0;
    }

    double percentage = (double)GC_count / total_count;
    return percentage;
}