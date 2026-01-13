
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_STEPS 100000 // Numero di passi

// Funzione per inizializzare i valori degli spin
void initialize_spins(int spins[][100], int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // Assegna -1 o 1
        }
    }
}

// Funzione per calcolare l'energia del sistema
double calculate_energy(int spins[][100], int N) {
    double energy = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            energy -= spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                     spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
        }
    }
    return energy / 2.0;  // Ogni interazione è stata contata due volte
}

// Funzione per eseguire un passo di Metropolis
void metropolis_step(int spins[][100], int N, double beta) {
    int i = rand() % N;
    int j = rand() % N;
    int delta_energy = 2 * spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                          spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
    if (delta_energy < 0 || ((double)rand() / RAND_MAX) < exp(-beta * delta_energy)) {
        spins[i][j] *= -1;
    }
}



int main() {
    srand((unsigned)time(NULL));  // Inizializza il generatore di numeri casuali

    // Definizione delle temperature e delle dimensioni
    double temperatures[] = {1.0, 2.5, 3.5};
    int sizes[] = {25, 50, 100};
    int num_temperatures = sizeof(temperatures) / sizeof(temperatures[0]);
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Iterare su tutte le combinazioni di temperature e dimensioni
    for (int t = 0; t < num_temperatures; ++t) {
        for (int s = 0; s < num_sizes; ++s) {
            int N = sizes[s];
            double beta = 1.0 / temperatures[t];
            
            // Inizializza la griglia con la dimensione attuale
            int spins[100][100];  // La dimensione 100 è il massimo previsto
            initialize_spins(spins, N);

            // Creazione del file per i dati
            char filename[50];
            snprintf(filename, sizeof(filename), "ising_data_T%.1f_N%d_random.txt", temperatures[t], N);
            FILE *file = fopen(filename, "w");
            if (file == NULL) {
                perror("Error opening file");
                return EXIT_FAILURE;
            }

            // Simulazione
            for (int step = 0; step < N_STEPS; ++step) {
                metropolis_step(spins, N, beta);
                double energy_metropolis = calculate_energy(spins, N);
                double magnetization_metropolis = 0.0;
                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j) {
                        magnetization_metropolis += spins[i][j];
                    }
                }
                magnetization_metropolis /= (N * N);  // Normalizzare

              
        }
    }

    return 0;
}
