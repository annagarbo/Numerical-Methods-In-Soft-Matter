#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_STEPS 100000 // Numero di passi

// Funzione per inizializzare gli spin con una probabilità di spin positivi
void initialize_spins(int **spins, int N, double probability) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            spins[i][j] = ((double)rand() / RAND_MAX) < probability ? 1 : -1;
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

    // Definizione delle temperature
    double T_critical = 2.0 / log(1.0 + sqrt(2.0)); // Temperatura critica
    double temperatures[] = {T_critical / 2.0, T_critical, 2.0 * T_critical}; // T_half, T_critical, T_double
    int sizes[] = {25,50,100};
    int num_temperatures = sizeof(temperatures) / sizeof(temperatures[0]);
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    double probability_of_positive = 0.75; // Probabilità di spin positivi (configurazione positiva)

    // Iterare su tutte le combinazioni di temperature e dimensioni
    for (int config = 0; config < 3; ++config) { // 0: positiva, 1: casuale, 2: negativa
        double probability = (config == 0) ? probability_of_positive : (config == 2 ? 0.25 : 0.5);
        const char *config_names[] = {"positive", "random", "negative"};

        for (int t = 0; t < num_temperatures; ++t) {
            for (int s = 0; s < num_sizes; ++s) {
                int N = sizes[s];
                double beta = 1.0 / temperatures[t];

                // Allocazione dinamica della griglia di spin
                int **spins = (int **)malloc(N * sizeof(int *));
                for (int i = 0; i < N; ++i) {
                    spins[i] = (int *)malloc(N * sizeof(int));
                }

                // Inizializzare la griglia in base alla configurazione desiderata
                if (config == 0) { // Configurazione positiva
                    initialize_spins(spins, N, probability_of_positive);
                } else if (config == 1) { // Configurazione casuale (50% positivi e negativi)
                    initialize_spins(spins, N, 0.5);
                } else { // Configurazione negativa
                    initialize_spins(spins, N, 0.25);
                }

                // Creazione del file per i dati
                char filename[50];
                snprintf(filename, sizeof(filename), "metro_%s_T%.1f_N%d.txt", config_names[config], temperatures[t], N);
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

                    // Calcola la magnetizzazione totale
                    for (int i = 0; i < N; ++i) {
                        for (int j = 0; j < N; ++j) {
                            magnetization_metropolis += spins[i][j];
                        }
                    }

                    // Normalizzazione della magnetizzazione ed energia
                    double normalized_magnetization = magnetization_metropolis / (N * N);
                    double normalized_energy = energy_metropolis / (N * N);

                    fprintf(file, "Metropolis %d %lf %lf\n", step, normalized_energy, normalized_magnetization);
                }

                fclose(file);

                // Libera la memoria allocata
                for (int i = 0; i < N; ++i) {
                    free(spins[i]);
                }
                free(spins);
            }
        }
    }

    return 0;
}

