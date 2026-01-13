//
//  circlehitmiss.c
//  Numerical Methods in Soft Matter
//
//  Created by Anna Garbo.1 on 29/10/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
    // Inizializza il generatore di numeri casuali con il tempo corrente
    srand(time(NULL));

    // Numero di punti da generare all'interno del cerchio
    int num_punti = 10000;

    // Apri il file "circle.csv" per la scrittura
    FILE *file = fopen("circle.csv", "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    // Intestazione del file CSV
    fprintf(file, "x,y\n");

    for (int i = 0; i < num_punti; i++) {
        // Genera numeri casuali tra -1 e 1
        double x = (double)rand() / RAND_MAX * 2 - 1;
        double y = (double)rand() / RAND_MAX * 2 - 1;

        // Verifica se il punto è all'interno del cerchio di raggio unitario
        if (x * x + y * y <= 1) {
            // Scrivi il punto nel file CSV
            fprintf(file, "%lf,%lf\n", x, y);
        }
    }

    // Chiudi il file
    fclose(file);

    return 0;
}

