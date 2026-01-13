//
//  inverse_method.c
//  
//
//  Created by Anna Garbo 1 on 29/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
    srand(12345);
    
    int num_samples = 1000000; // Numero di punti da generare

        // Calcola l'integrale della funzione non normalizzata nell'intervallo [0, 1]
        double integral = 0.0;
        for (double x = 0; x <= 1; x += 0.001) {
            integral += pow(x, 3) * 0.001; // Utilizza una piccola incrementazione per calcolare l'integrale
        }
    printf("%lf\n",  integral);

    FILE *file = fopen("gaussiana", "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    for (int i = 0; i < num_samples; i++) {
        // Genera una variabile casuale "u" tra 0 e 2
        double u =(double)rand() / RAND_MAX;

        // Calcola il punto "x" secondo la legge di potenza y = x^3
        double x =sqrt(1-log(3*u+1));
        
        

        fprintf(file, "%lf\n", x); // Scrivi il punto "x" nel file
    }

    fclose(file);

    return 0;
}
