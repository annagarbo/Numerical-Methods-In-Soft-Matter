#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_STEPS 100000 // Numero di passi

// Funzione per inizializzare gli spin con una bias verso 1
void initialize_spins(int **spins, int N, double probability_of_1) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            spins[i][j] = ((double)rand() / RAND_MAX) < probability_of_1 ? 1 : -1;
        }
    }
}

// Funzione per calcolare l'energia del sistema
double calculate_energy(int **spins, int N) {
    double energy = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            energy -= spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                     spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);
        }
    }
    return energy / 2.0;  // Ogni interazione è contata due volte
}

// Funzione per eseguire un passo di Metropolis
void metropolis_step(int **spins, int N, double beta) {
    int i = rand() % N;
    int j = rand() % N;
    int delta_energy = 2 * spins[i][j] * (spins[(i+1)%N][j] + spins[(i-1+N)%N][j] +
                                          spins[i][(j+1)%N] + spins[i][(j-1+N)%N]);

    // Verifica condizione di Metropolis
    if (delta_energy < 0 || ((double)rand() / RAND_MAX) < exp(-beta * delta_energy)) {
        spins[i][j] = -spins[i][j];  // Flippa lo spin
    }
}

int main() {
    srand((unsigned)time(NULL));  // Inizializza il generatore di numeri casuali

    // Definizione delle temperature e delle dimensioni
    double temperatures[] = {1.0,1.4,1.5,1.7,1.9,2.1,2.4, 2.6,2.7,2.9,3.1,3.3, 3.5,3.7,3.9 };
    int sizes[] = {16};
    int num_temperatures = sizeof(temperatures) / sizeof(temperatures[0]);
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    double probability_of_1 = 0.5; // 50% di spin positivi

    // Iterare su tutte le combinazioni di temperature e dimensioni
    for (int t = 0; t < num_temperatures; ++t) {
        for (int s = 0; s < num_sizes; ++s) {
            int N = sizes[s];
            double beta = 1.0 / temperatures[t];
            
            // Allocazione dinamica della griglia di spin
            int **spins = (int **)malloc(N * sizeof(int *));
            for (int i = 0; i < N; ++i) {
                spins[i] = (int *)malloc(N * sizeof(int));
            }

            // Inizializzare la griglia con la dimensione attuale
            initialize_spins(spins, N, probability_of_1);

            // Creazione del file per i dati
            char filename[50];
            snprintf(filename, sizeof(filename), "rew_T%.1f_N%d.txt", temperatures[t], N);
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

                fprintf(file, "%d %lf %lf\n", step, energy_metropolis, magnetization_metropolis);
            }

            fclose(file);

            // Libera la memoria allocata
            for (int i = 0; i < N; ++i) {
                free(spins[i]);
            }
            free(spins);
        }
    }

    return 0;
}

