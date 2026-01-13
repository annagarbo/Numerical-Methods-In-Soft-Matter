

#include <stdio.h>
#include <stdlib.h>

#define N 100 // Numero di punti dati
#define BOX_LENGTH 1.0 // Lunghezza totale della scatola

// Dichiarazione delle funzioni
void simulate(double* densities, double* pressures, double* positions, int n, double temperature);
void write_data(const char* filename, double* densities, double* pressures, double* positions, int n, double temperature);

int main() {
    double densities[N];
    double pressures[N];
    double positions[N]; // Aggiungi una variabile per la posizione

    // Temperature da simulare
    double temperatures[] = {0.9, 2.0};
    const char* filenames[] = {"results_T1.txt", "results_T2.txt"};
    
    for (int t = 0; t < 2; ++t) {
        // Simula i dati per la temperatura corrente
        simulate(densities, pressures, positions, N, temperatures[t]);

        // Scrivi i dati nel file per la temperatura corrente
        write_data(filenames[t], densities, pressures, positions, N, temperatures[t]);
    }

    return 0;
}

void simulate(double* densities, double* pressures, double* positions, int n, double temperature) {
    // Simula i dati (qui inserisci il tuo codice di simulazione)
    for (int i = 0; i < n; ++i) {
        positions[i] = (double)i / n * BOX_LENGTH; // Esempio di posizione simulata
        densities[i] = positions[i] * temperature; // Esempio di densità simulata (dipendente dalla posizione e dalla temperatura)
        pressures[i] = densities[i] * densities[i] / temperature; // Esempio di pressione simulata (parabola modificata dalla temperatura)
    }
}

void write_data(const char* filename, double* densities, double* pressures, double* positions, int n, double temperature) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    double cutoff = BOX_LENGTH / 2.0; // Calcola il valore del cut-off

    for (int i = 0; i < n; ++i) {
        // Determina se la posizione è prima o dopo il cut-off
        if (positions[i] <= cutoff) {
            fprintf(file, "%.6f %.6f %.6f %d %.1f\n", densities[i], pressures[i], positions[i], 0, temperature); // 0 indica prima del cut-off
        } else {
            fprintf(file, "%.6f %.6f %.6f %d %.1f\n", densities[i], pressures[i], positions[i], 1, temperature); // 1 indica dopo il cut-off
        }
    }

    fclose(file);
}
