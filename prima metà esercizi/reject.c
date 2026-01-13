//
//  reject.c
//  
//
//  Created by Anna Garbo 1 on 31/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Funzione per generare un numero casuale tra 0 e 1
double random_uniform() {
    return (double)rand() / RAND_MAX;
}

// PDF target f(x)
double target_pdf(double x) {
    return sqrt(2.0 / M_PI) * exp(-x * x);
}

// PDF di confronto g(x) con un valore specifico di p
double comparison_pdf(double x, double p) {
    
    if (x < p) {
        return 2*p/(2*p*p+1);
    } else {
        return 2/(2*p*p+1) * x * exp(p * p - x * x);
    }
}

int main() {
    FILE *file = fopen("reject1.csv", "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    int num_samples = 10000;
    double x, y, u;
    double p = 0.3;
    double A=2*p/(2*p*p+1);
    printf("%f\n", A);
    srand(42);
 
    for (int i = 0; i < num_samples; i++) {
        while (1) {
            u = random_uniform();
            y = random_uniform();

            if (u < 2 * p / (2 * p * p + 1)) {
                x = (2 * p * p + 1) * u / (2 * p);
            } else {
                x = sqrt(p * p - log(1 - (2 * p * p + 1) * u + 2 * p * p));
            }

            if (y <= target_pdf(x) / (1.15*comparison_pdf(x, p))) {
                break;  // Esci dal ciclo while solo se la condizione è soddisfatta
            }
        }

        fprintf(file, "%f\n", x);
    }

    fclose(file);

    return 0;
}
