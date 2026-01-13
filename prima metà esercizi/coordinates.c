//
//  coordinates.c
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

// Funzione per campionare punti all'interno del disco unitario
void sample_points_in_unit_disk(double* x, double* y) {
    double r, theta;

    // Genera un valore casuale r^2 in [0, 1]
    double u1 = random_uniform();
    r = sqrt(u1);

    // Genera un valore casuale per l'angolo tra 0 e 2*pi
    double u2 = 2.0 * M_PI * random_uniform();
    theta = u2;

    // Converte le coordinate polari in coordinate cartesiane
    *x = r * cos(theta);
    *y = r * sin(theta);
}

int main() {
    
    // Apri il file "circle.csv" per la scrittura
    FILE *file = fopen("coord1.csv", "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }
    
    
    int num_samples = 10000;
    double x, y;

    // Inizializza il generatore di numeri casuali con un seme
    srand(1234);

    for (int i = 0; i < num_samples; i++) {
        sample_points_in_unit_disk(&x, &y);
        fprintf(file, "%lf,%lf\n", x, y);
    }

    return 0;
}


